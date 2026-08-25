#include "packcompiler.h"
#include "common/exceptions.h"

PackCompiler::PackCompiler(QObject *parent)
    : QObject{parent}
{
    if(m_variableExpressions.isEmpty())
    {
        for(const QString &variable : m_updateVariables)
        {
            m_variableExpressions.append
            (
                QRegularExpression
                (
                    QStringLiteral("\\b(?<!ubuf\\.)%1\\b").arg
                    (
                        variable
                    )
                )
            );
        }
    }
}

auto PackCompiler::reset() -> void
{
    setError({}, {});
    setProgress(std::numeric_limits<qreal>::infinity());
    setTotalSteps(0);
    setCurrentStep(0);
    setCompilerOutput({});
    setStatus({});
    setState(Idle);
}

auto PackCompiler::build(const QUrl &uri) -> QUrl
{
    BuildContext *context = new BuildContext;
    QUrl extractedUri = extract(uri);

    validatePack(extractedUri);
    context->commonData = loadCommonData(extractedUri);
    processShaders(extractedUri, context);

    delete context;

    return extractedUri;
}

auto PackCompiler::extract(const QUrl &sourceUri) noexcept(false) -> const QUrl
{
    validateUri(sourceUri);

    QFileInfo info(sourceUri.toLocalFile());

    QUrl outputUri
    (
        QStringLiteral("file:///tmp/%1").arg
        (
            info.baseName()
        )
    );

    QStringList arguments =
    {
        QStringLiteral("-xzf"),
        sourceUri.toLocalFile(),
        QStringLiteral("-C"),
        outputUri.toLocalFile()
    };

    QDir outputDir(outputUri.toLocalFile());

    if(!outputDir.exists())
    {
        createDirectory(outputUri);
    }

    run(QStringLiteral("tar"), arguments);

    QFile::remove(sourceUri.toLocalFile());

    return std::move(outputUri);
}

auto PackCompiler::validatePack(const QUrl &uri) const noexcept(false) -> void
{
    QDir directory(uri.toLocalFile());

    if (!directory.exists(QStringLiteral("pack.json")))
    {
        throw shader::logic_error("Compiler Error", QStringLiteral("Invalid pack file").toStdString(), ENOEXEC);
    }

    if (!directory.exists(QStringLiteral("shaders")))
    {
        throw shader::logic_error("Compiler Error", QStringLiteral("Nothing to compile").toStdString(), ENOEXEC);
    }
}

auto PackCompiler::loadCommonData(const QUrl &uri) -> QMap<QByteArray,QByteArray>
{
    validateUri(uri);

    QMap<QByteArray,QByteArray> commonData;

    QDir packDirectory(uri.toLocalFile());
    QDir shaderDirectory(packDirectory.absoluteFilePath(QStringLiteral("shaders")));
    shaderDirectory.setNameFilters(m_shaderNameFilters);
    shaderDirectory.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    const QStringList entries = shaderDirectory.entryList();

    for(const QString &entry : entries)
    {
        if(entry.startsWith(QStringLiteral("common"), Qt::CaseInsensitive))
        {
            QFileInfo entryInfo(shaderDirectory.absoluteFilePath(entry));
            QByteArray data = commonData.take(entryInfo.suffix().toUtf8());
            data += readFile
            (
                QStringLiteral("file://%1").arg
                (
                    entryInfo.absoluteFilePath()
                )
            );

            commonData.insert(entryInfo.suffix().toUpper().toUtf8(), data);

            QFile::remove(entryInfo.absoluteFilePath());
        }

        else if(entry.toLower() == QStringLiteral("global.glsl"))
        {
            if(commonData.contains(QByteArray("GLOBAL")))
            {
                commonData.remove(QByteArray("GLOBAL"));
            }

            QFileInfo entryInfo(shaderDirectory.absoluteFilePath(entry));
            QByteArray data = readFile
            (
                QStringLiteral("file://%1").arg
                (
                    entryInfo.absoluteFilePath()
                )
            );

            commonData.insert(QByteArray("GLOBAL"), data);

            QFile::remove(entryInfo.absoluteFilePath());
        }
    }

    return std::move(commonData);
}

auto PackCompiler::processShaders(const QUrl &uri, BuildContext *context) -> void
{
    validateUri(uri);

    QDir packDirectory(uri.toLocalFile());
    QDir shaderDirectory(packDirectory.absoluteFilePath(QStringLiteral("shaders")));
    shaderDirectory.setNameFilters(m_shaderNameFilters);
    shaderDirectory.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    const QStringList entries = shaderDirectory.entryList();

    for(const QString &entry : entries)
    {
        QFileInfo entryInfo(shaderDirectory.absoluteFilePath(entry));
        QByteArray shaderData;
        QUrl entryUri = QStringLiteral("file://%1").arg
        (
            entryInfo.absoluteFilePath()
        );

        shaderData += readFile(entryUri);

        QShader::Stage stage = getStageFromSuffix(entryInfo.suffix());

        appendCommonData(&shaderData, entryInfo.suffix().toUtf8(), context);
        replaceUniformVariables(&shaderData);
        appendUniformHeader(&shaderData);
        appendUniformFooter(&shaderData);
        preprocess(&shaderData);
        appendVersion(&shaderData);

        QUrl outputUri = QStringLiteral
        (
            "file://%1.qsb"
        ).arg(entryInfo.absoluteFilePath());

        QShader shader = compile(shaderData, outputUri.fileName(), stage);
        save(outputUri, shader);

        QFile::remove(entryUri.toLocalFile());
    }
}

auto PackCompiler::preprocess(QByteArray *data) noexcept(false) -> void
{
    QStringList arguments;
    arguments << "-E"        // Run only the preprocessor
              << "-P"        // Omit #line markers
              << "-x" << "c++"
              << "-";        // Read source from stdin

    QByteArray temp = run(QStringLiteral("clang"), arguments, *data);
    data->clear();
    data->swap(temp);
}

auto PackCompiler::appendVersion(QByteArray *data) noexcept(true) -> void
{
    data->push_front(m_version + QByteArray("\n\n"));
}

auto PackCompiler::compile(const QByteArray &data, const QString &filename, const QShader::Stage stage) noexcept(false) -> QShader
{
    QList<QShaderBaker::GeneratedShader> targets;

#ifdef HAS_VULKAN
    targets.append({ QShader::SpirvShader, QShaderVersion(vulkanVersion()) });
#endif

#ifdef HAS_OPENGL
    targets.append({ QShader::GlslShader, QShaderVersion(openGlVersion()) });
#endif

    QShaderBaker baker;
    baker.setGeneratedShaderVariants({ QShader::StandardShader });
    baker.setGeneratedShaders(targets);
    baker.setSourceString(data, stage, filename);

    QShader shader = baker.bake();

    if(!shader.isValid())
    {
        QUrl errorUri
        (
            QStringLiteral("file://tmp/_err")
        );

        writeFile(errorUri, data);

        throw shader::logic_error
        (
            std::string("Compiler Error"),
            baker.errorMessage().toStdString()
        );
    }

    return shader;
}

#ifdef HAS_VULKAN
auto PackCompiler::vulkanVersion() const -> qint32
{
    uint32_t instanceVersion = VK_API_VERSION_1_0;
    auto FN_vkEnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
    if(FN_vkEnumerateInstanceVersion)
    {
        FN_vkEnumerateInstanceVersion(&instanceVersion);
    }

    uint32_t major = VK_VERSION_MAJOR(instanceVersion);
    uint32_t minor = VK_VERSION_MINOR(instanceVersion);

    QString versionString = QStringLiteral("%1%2%3").arg(major).arg(minor).arg(0);
    return versionString.toInt();
}
#endif

auto PackCompiler::save(const QUrl &uri, const QShader &shader) noexcept(false) -> void
{
    writeFile(uri, shader.serialized());
}

auto PackCompiler::appendCommonData(QByteArray *data, const QByteArray &suffix, BuildContext *context) noexcept(true) -> void
{
    if(context->commonData.contains(QByteArray("GLOBAL")))
    {
        data->push_front
        (
            context->commonData.value(QByteArray("GLOBAL")) + QByteArray("\n")
        );
    }

    if(context->commonData.contains(suffix.toUpper()))
    {
        data->push_front
        (
            context->commonData.value(suffix.toUpper()) + QByteArray("\n")
        );
    }
}

auto PackCompiler::appendUniformHeader(QByteArray *data) noexcept(true) -> void
{
    data->push_front(m_header + QByteArray("\n\n"));
}

auto PackCompiler::appendUniformFooter(QByteArray *data) noexcept(true) -> void
{
    data->push_back(QByteArray("\n\n") + m_footer);
}

auto PackCompiler::replaceUniformVariables(QByteArray *data) noexcept(true) -> void
{
    for(const QRegularExpression &expression : std::as_const(m_variableExpressions))
    {
        auto match = expression.match(*data);

        if(match.hasMatch())
        {
            QByteArray replacement = QByteArray("ubuf.") + match.captured().toLocal8Bit();
            data->replace(match.captured().toLocal8Bit(), replacement);
        }
    }
}

auto PackCompiler::copyFile(const QUrl &sourceUri, const QUrl &destinationUri) noexcept(false) -> void
{
    if(!QFile::exists(sourceUri.toLocalFile()))
    {
        throw file_exception(QStringLiteral("File does not exist").toStdString(), ENOENT);
    }

    if(!QFile::copy(sourceUri.toLocalFile(), destinationUri.toLocalFile()))
    {
        throw file_exception(QStringLiteral("Could not copy file").toStdString());
    }
}

auto PackCompiler::createDirectory(const QUrl &uri) -> void
{
    validateUri(uri);

    QStringList arguments =
    {
        QStringLiteral("-p"),
        uri.toLocalFile()
    };

    run(QStringLiteral("mkdir"), arguments);
}

auto PackCompiler::removeDirectory(const QUrl &uri) noexcept(false) -> void
{
    QStringList arguments =
    {
        QStringLiteral("-rf"),
        uri.toLocalFile()
    };

    run(QStringLiteral("rm"), arguments);
}

auto PackCompiler::getStageFromSuffix(const QString &suffix) const -> QShader::Stage
{
    return m_shaderStages.value(suffix.toLower(), QShader::FragmentStage);
}

#ifdef HAS_OPENGL
auto PackCompiler::openGlVersion() const -> qint32
{
    qint32 glVersion = 330;

    QString versionString = QString::fromUtf8(glGetString(GL_SHADING_LANGUAGE_VERSION));
    QStringList parts = versionString.split(QChar(' '), Qt::SkipEmptyParts);

    if(parts.count() >= 1)
    {
        QString version = parts.at(0);
        version.remove(QChar('.'));

        bool okay = true;
        qint32 temp = version.toInt(&okay);

        if(okay)
        {
            glVersion = temp;
        }
    }

    return glVersion;
}
#endif

auto PackCompiler::validateUri(const QUrl &uri) const noexcept(false) -> void
{
    if(!uri.isValid() || !uri.isLocalFile())
    {
        throw file_exception
        (
            QStringLiteral("URI %1 is not a local file uri").arg
            (
                uri.toString()
            ).toStdString(),
            std::error_code
            (
                ENOENT,
                std::system_category()
            )
        );
    }
}

auto PackCompiler::run(const QString &command, const QStringList &arguments, const QByteArray &data) noexcept(false) -> QByteArray
{
    QProcess process;
    process.start(command, arguments);

    if(!process.waitForStarted(3000))
    {
        throw process_exception
        (
            QStringLiteral("Could not start process for %1").arg(command).toStdString(),
            process.errorString().toStdString()
        );
    }

    if(data.length() > 0)
    {
        process.write(data);
        process.closeWriteChannel();
    }

    if(!process.waitForFinished())
    {
        throw process_exception
        (
            QStringLiteral("Process timeout for %1").arg(command).toStdString(),
            process.errorString().toStdString()
        );
    }

    if(process.exitCode() != 0)
    {
        throw process_exception
        (
            QStringLiteral("Process exited abnormally: %1").arg
            (
                process.readAllStandardError()
            ).toStdString(),
            process.errorString().toStdString(),
            std::error_code
            (
                process.exitCode(),
                std::system_category()
            )
        );
    }

    return process.readAllStandardOutput();
}

auto PackCompiler::readFile(const QUrl &uri) -> QByteArray
{
    validateUri(uri);
    QByteArray data;
    QFile file(uri.toLocalFile());

    if(!file.open(QFile::ReadOnly))
    {
        throw file_exception
        (
            QStringLiteral("Could not file").toStdString(),
            std::error_code
            (
                errno,
                std::system_category()
            )
        );
    }

    data = file.readAll();

    if(data.length() != file.size())
    {
        throw file_exception
        (
            QStringLiteral("File read error").toStdString(),
            std::error_code
            (
                EIO,
                std::system_category()
            )
        );
    }

    file.close();
    return data;
}

auto PackCompiler::writeFile(const QUrl &uri, const QByteArray &data) noexcept(false) -> void
{
    validateUri(uri);

    QFile file(uri.toLocalFile());
    if(!file.open(QFile::ReadWrite))
    {
        throw file_exception
        (
            QStringLiteral("Could not file").toStdString(),
            std::error_code
            (
                errno,
                std::system_category()
            )
        );
    }

    file.write(data);

    if(data.length() != file.size())
    {
        throw file_exception
        (
            QStringLiteral("File write error").toStdString(),
            std::error_code
            (
                EIO,
                std::system_category()
            )
        );
    }

    file.close();
}

auto PackCompiler::setProgress(qreal progress) -> void
{
    if(qFuzzyCompare(progress, m_progress))
    {
        return;
    }

    m_progress = progress;
    Q_EMIT progressChanged();
}

void PackCompiler::setCompilerOutput(const QString &compilerOutput)
{
    if (m_compilerOutput == compilerOutput)
    {
        return;
    }

    m_compilerOutput = compilerOutput;
    emit compilerOutputChanged();
}

auto PackCompiler::setError(const QString &title, const QString &message, const QUrl &uri) -> void
{
    if(title == m_errorTitle && message == m_errorMessage)
    {
        return;
    }

    m_errorMessage = std::move(message);
    m_errorTitle = std::move(title);

    if(uri.isValid() && uri.isLocalFile())
    {
        QFile errorFile(uri.toLocalFile());

        if(errorFile.open(QFile::ReadWrite | QFile::Append))
        {
            errorFile.write(message.toLocal8Bit() + QByteArray("\n"));
        }
    }

    Q_EMIT errorTitleChanged();
    Q_EMIT errorMessageChanged();
    Q_EMIT errorOcurred();
}

auto PackCompiler::setState(State state) -> void
{
    if(state == m_state)
    {
        return;
    }

    m_state = state;
    Q_EMIT stateChanged();
}

auto PackCompiler::setStatus(const QString &status) -> void
{
    if (m_status == status)
    {
        return;
    }

    m_status = status;
    Q_EMIT statusChanged();
}

auto PackCompiler::setTotalSteps(qint64 steps) -> void
{
    if(steps == m_totalSteps)
    {
        return;
    }

    m_totalSteps = steps;
    Q_EMIT totalStepsChanged();
}

auto PackCompiler::setCurrentStep(qint64 step) -> void
{
    if(step == m_currentStep)
    {
        return;
    }

    m_currentStep = step;
    Q_EMIT currentStepChanged();
}

auto PackCompiler::incrementCompileStep() -> void
{
    setCurrentStep(m_currentStep + 1);

    setProgress
    (
        static_cast<qreal>(m_currentStep) / m_totalSteps
    );
}
