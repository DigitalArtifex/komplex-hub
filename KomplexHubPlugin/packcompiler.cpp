#include "packcompiler.h"
#include "common/exceptions.h"
#include <utility>

PackCompiler::PackCompiler(QObject *parent)
    : QObject(parent)
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

PackCompiler::~PackCompiler() = default;

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

auto PackCompiler::process(const QUrl &uri) -> QFuture<QUrl>
{
    return QtConcurrent::run
    (
        [this, uri] () -> QUrl
        {
            if(!uri.isLocalFile())
            {
                setError
                (
                    QStringLiteral("File Error"),
                    QStringLiteral("URI %1 is not a local file").arg
                    (
                        uri.toString()
                    )
                );

                return {};
            }

            reset();
            setStatus(QStringLiteral("Compiling %1").arg(uri.fileName()));
            setState(Compiling);

            // this was a leftover from ThumbnailGenerator
            // if(!validateDirectory(uri))
            // {
            //     throw FileException
            //     (
            //         QStringLiteral("Source directory is invalid")
            //     );
            // }

            QFileInfo info(uri.toLocalFile());

            QUrl buildUri;

            try
            {
                buildUri = extract(uri);
                prepareShaders(buildUri);
                compile(buildUri);
            }
            catch (FileException e)
            {
                removeDirectory(buildUri);
                setError(QStringLiteral("File Error"), e.message);
                throw e;
            }
            catch (ShaderCompilerException e)
            {
                removeDirectory(buildUri);
                setError(QStringLiteral("Compiler Error"), e.message);
                throw e;
            }
            catch (...)
            {
                removeDirectory(buildUri);
            }

            return buildUri;
        }
    );
}

auto PackCompiler::prepareShaders(const QUrl &uri) noexcept(false) -> void
{
    if(!uri.isLocalFile())
    {
        throw FileException
        (
            QStringLiteral("URI %1 is not a local file uri").arg
            (
                uri.toString()
            )
        );
    }

    QDir sourceDirectory(uri.toLocalFile());
    QDir shaderDirectory
    (
        sourceDirectory.absoluteFilePath
        (
            QStringLiteral("shaders")
        )
    );

    QStringList entries = shaderDirectory.entryList
    (
        QDir::NoDotAndDotDot | QDir::Files
    );

    setTotalSteps(entries.count() * 2);
    setProgress(0);

    QByteArray commonFragmentData = loadCommonFragmentData(uri);

    if(commonFragmentData.length() > 0)
    {
        setTotalSteps(m_totalSteps -= 1);
        incrementCompileStep();
    }

    QByteArray commonVertexData = loadCommonVertexData(uri);

    if(commonVertexData.length() > 0)
    {
        setTotalSteps(m_totalSteps -= 1);
        incrementCompileStep();
    }

    QByteArray globalData = loadGlobalData(uri);

    if(globalData.length() > 0)
    {
        setTotalSteps(m_totalSteps -= 1);
        incrementCompileStep();
    }

    for(const QString &entry : std::as_const(entries))
    {
        auto commonFragMatch = m_commonFragmentExpression.match(entry);
        auto commonVertMatch = m_commonVertexExpression.match(entry);

        if(commonFragMatch.hasMatch() || commonVertMatch.hasMatch())
        {
            continue;
        }

        auto fragMatch = m_fragmentExpression.match(entry);
        auto vertMatch = m_vertexExpression.match(entry);

        QFile file(shaderDirectory.absoluteFilePath(entry));

        if(!file.open(QFile::ReadWrite))
        {
            throw FileException
            (
                QStringLiteral("Could not open file %1 for preperation").arg
                (
                    shaderDirectory.absoluteFilePath(entry)
                )
            );
        }

        QByteArray fileData = file.readAll();

        if(fileData.length() != file.size())
        {
            throw FileException
            (
                QStringLiteral("Could not read file %1 for preperation").arg
                (
                    shaderDirectory.absoluteFilePath(entry)
                )
            );
        }

        file.close();

        QByteArray commonData;

        if(fragMatch.hasMatch())
        {
            commonData = commonFragmentData;
        }
        else if(vertMatch.hasMatch())
        {
            commonData = commonVertexData;
        }
        else
        {
            throw FileException
            (
                QStringLiteral("File %1 is an unrecognized shader type (frag/vert)").arg
                (
                    shaderDirectory.absoluteFilePath(entry)
                )
            );
        }

        QByteArray combinedData = QByteArray
        (
            globalData + QByteArray("\n") +
            commonData + QByteArray("\n") +
            fileData
        );

        for(const QRegularExpression &expression : std::as_const(m_variableExpressions))
        {
            auto match = expression.match(combinedData);

            if(match.hasMatch())
            {
                QByteArray replacement = QByteArray("ubuf.") + match.captured().toLocal8Bit();
                combinedData.replace(match.captured().toLocal8Bit(), replacement);
            }
        }

        QByteArray preparedData = QByteArray
        (
            m_header.toLocal8Bit() + QByteArray("\n") +
            combinedData + QByteArray("\n") +
            m_footer.toLocal8Bit()
        );

        if(!file.open(QFile::ReadWrite | QFile::Truncate))
        {
            throw FileException
            (
                QStringLiteral("Could not open file %1 for writing preperation").arg
                (
                    shaderDirectory.absoluteFilePath(entry)
                )
            );
        }

        if(file.write(preparedData) != preparedData.length())
        {
            throw FileException
            (
                QStringLiteral("Could not write file %1 preperation").arg
                (
                    shaderDirectory.absoluteFilePath(entry)
                )
            );
        }

        file.close();
        incrementCompileStep();
    }
}

auto PackCompiler::loadCommonFragmentData(const QUrl &uri) -> QByteArray
{
    if(!uri.isLocalFile())
    {
        throw FileException
        (
            QStringLiteral("URI %1 is not a local file uri").arg
            (
                uri.toString()
            )
        );
    }

    QDir sourceDirectory
    (
        QStringLiteral("%1/shaders/").arg
        (
            uri.toLocalFile()
        )
    );

    const QStringList entries = sourceDirectory.entryList
    (
        QDir::NoDotAndDotDot | QDir::Files
    );

    QByteArray data;

    for(const QString &entry : entries)
    {
        if (entry.toLower() != QStringLiteral("common.frag"))
        {
            continue;
        }

        QFile file(sourceDirectory.absoluteFilePath(entry));

        if(!file.open(QFile::ReadWrite))
        {
            throw FileException(QStringLiteral("Could not open common shader data"));
        }

        data = file.readAll();

        if(data.length() != file.size())
        {
            throw FileException(QStringLiteral("Common fragment shader file read error"));
        }

        file.close();
        break;
    }

    return data;
}

auto PackCompiler::loadCommonVertexData(const QUrl &uri) -> QByteArray
{
    if(!uri.isLocalFile())
    {
        throw FileException
        (
            QStringLiteral("URI %1 is not a local file uri").arg
            (
                uri.toString()
            )
        );
    }

    QDir sourceDirectory
    (
        QStringLiteral("%1/shaders/").arg
        (
            uri.toLocalFile()
        )
    );

    const QStringList entries = sourceDirectory.entryList
    (
        QDir::NoDotAndDotDot | QDir::Files
    );

    QByteArray data;

    for(const QString &entry : entries)
    {
        if (entry.toLower() != QStringLiteral("common.vert"))
        {
            continue;
        }

        QFile file(sourceDirectory.absoluteFilePath(entry));

        if(!file.open(QFile::ReadWrite))
        {
            throw FileException(QStringLiteral("Could not open common vertex shader data"));
        }

        data = file.readAll();

        if(data.length() != file.size())
        {
            throw FileException(QStringLiteral("Common vertex shader file read error"));
        }

        file.close();
        break;
    }

    return data;
}

auto PackCompiler::loadGlobalData(const QUrl &uri) -> QByteArray
{
    if(!uri.isLocalFile())
    {
        throw FileException
        (
            QStringLiteral("URI %1 is not a local file uri").arg
            (
                uri.toString()
            )
        );
    }

    QDir sourceDirectory
    (
        QStringLiteral("%1/shaders/").arg
        (
            uri.toLocalFile()
        )
    );

    const QStringList entries = sourceDirectory.entryList
    (
        QDir::NoDotAndDotDot | QDir::Files
    );

    QByteArray data;

    for(const QString &entry : entries)
    {
        if (entry.toLower() != QStringLiteral("global.glsl"))
        {
            continue;
        }

        QFile file(sourceDirectory.absoluteFilePath(entry));

        if(!file.open(QFile::ReadWrite))
        {
            throw FileException
            (
                QStringLiteral("Could not open global shader data")
            );
        }

        data = file.readAll();

        if(data.length() != file.size())
        {
            throw FileException
            (
                QStringLiteral("Global shader file read error")
            );
        }

        file.close();
        break;
    }

    return data;
}

auto PackCompiler::validateDirectory(const QUrl &uri) -> bool
{
    QDir directory(uri.toLocalFile());

    return
    (
        directory.exists(QStringLiteral("shaders")) &&
        directory.exists(QStringLiteral("pack.json"))
    );
}

auto PackCompiler::createDirectory(const QUrl &uri) -> void
{
    if(!uri.isLocalFile() || !uri.isValid())
    {
        throw FileException(QStringLiteral("Uri needs to be a local file"), 0);
    }

    QStringList arguments =
    {
        QStringLiteral("-p"),
        uri.toLocalFile()
    };

    QProcess *process = new QProcess(this);

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardOutput,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardOutput();
            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardError,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardError();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    process->start(QStringLiteral("mkdir"), arguments);

    if(!process->waitForStarted(3000))
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Could not start preprocessor"));
    }

    if(!process->waitForFinished())
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Preprocessor timeout"));
    }

    if(process->exitCode() != 0)
    {
        process->deleteLater();
        throw ShaderCompilerException(m_compilerOutput);
    }

    process->deleteLater();
}

auto PackCompiler::extract(const QUrl &sourceUri) noexcept(false) -> QUrl
{
    if(!sourceUri.isLocalFile() || !sourceUri.isValid())
    {
        throw FileException(QStringLiteral("Uri needs to be a local file"), 0);
    }

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

    QProcess *process = new QProcess(this);

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardOutput,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardOutput();
            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardError,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardError();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    process->start(QStringLiteral("tar"), arguments);

    if(!process->waitForStarted(3000))
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Could not start preprocessor"));
    }

    if(!process->waitForFinished())
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Preprocessor timeout"));
    }

    if(process->exitCode() != 0)
    {
        process->deleteLater();
        throw ShaderCompilerException(m_compilerOutput);
    }

    process->deleteLater();

    QFile::remove(sourceUri.toLocalFile());

    return outputUri;
}

auto PackCompiler::copyFile(const QUrl &sourceUri, const QUrl &destinationUri) noexcept(false) -> void
{
    if(!QFile::exists(sourceUri.toLocalFile()))
    {
        return;
    }

    if(!QFile::copy(sourceUri.toLocalFile(), destinationUri.toLocalFile()))
    {
        throw FileException(QStringLiteral("Could not copy file"));
    }
}

auto PackCompiler::compile(const QUrl &uri) noexcept(false) -> void
{
    if(!uri.isLocalFile() || !uri.isValid())
    {
        throw FileException(QStringLiteral("Uri needs to be a local file"), 0);
    }

    setState(Compiling);

    QDir packDirectory(uri.toLocalFile());
    QDir shaderDirectory
    (
        packDirectory.absoluteFilePath
        (
            QStringLiteral("shaders")
        )
    );

    shaderDirectory.setNameFilters
    (
        {
            QStringLiteral("*.frag"),
            QStringLiteral("*.vert")
        }
    );

    const QStringList shaders = shaderDirectory.entryList
    (
        QDir::Files | QDir::NoDotAndDotDot
    );

    for(const QString &shader : shaders)
    {
        if(shader.startsWith(QStringLiteral("common."), Qt::CaseInsensitive))
        {
            continue;
        }

        QUrl shaderUri
        (
            QStringLiteral("file://%1").arg
            (
                shaderDirectory.absoluteFilePath(shader)
            )
        );

        preprocess(shaderUri);
        appendVersion(shaderUri);
        compileShader(shaderUri);

        incrementCompileStep();
    }
}

auto PackCompiler::preprocess(const QUrl &uri) noexcept(false) -> void
{
    if(!uri.isLocalFile() || !uri.isValid())
    {
        throw FileException(QStringLiteral("Uri needs to be a local file"), 0);
    }

    QUrl outputUri
    (
        QStringLiteral("file://%1_proc").arg
        (
            uri.toLocalFile()
        )
    );

    QStringList arguments =
    {
        QStringLiteral("-P"),
        uri.toLocalFile(),
        outputUri.toLocalFile()
    };

    QProcess *process = new QProcess(this);

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardOutput,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardOutput();
            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardError,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardError();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    process->start(QStringLiteral("cpp"), arguments);

    if(!process->waitForStarted(3000))
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Could not start preprocessor"));
    }

    if(!process->waitForFinished())
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Preprocessor timeout"));
    }

    if(process->exitCode() != 0)
    {
        process->deleteLater();
        throw ShaderCompilerException(m_compilerOutput);
    }

    process->deleteLater();

    QFile::remove(uri.toLocalFile());
    QFile::rename(outputUri.toLocalFile(), uri.toLocalFile());
}

auto PackCompiler::appendVersion(const QUrl &uri) noexcept(false) -> void
{
    if(!uri.isValid() || !uri.isLocalFile())
    {
        throw FileException(QStringLiteral("File is not a valid local file"));
    }

    QFile file(uri.toLocalFile());

    if(!file.open(QFile::ReadWrite))
    {
        throw FileException(QStringLiteral("Could not open file for appending"));
    }

    QByteArray data = file.readAll();

    file.seek(0);

    file.write(m_version);
    file.write(QByteArray("\n\n"));
    file.write(data);

    file.close();
}

auto PackCompiler::compileShader(const QUrl &uri) noexcept(false) -> void
{
    if(!uri.isLocalFile())
    {
        throw FileException
        (
            QStringLiteral("Uri needs to be a local file")
        );
    }

    QString qsb = QStringLiteral("/usr/lib/qt6/bin/qsb");

    if(!QFile::exists(qsb))
    {
        throw FileException(QStringLiteral("QSB missing"));
    }

    QUrl outputUri
    (
        QStringLiteral("file://%1.qsb").arg
        (
            uri.toLocalFile()
        )
    );

    QStringList arguments =
    {
        QStringLiteral("--glsl"),
        QStringLiteral("330es, 330, 440"),
        QStringLiteral("-o"),
        outputUri.toLocalFile(),
        uri.toLocalFile()
    };

    QProcess *process = new QProcess(this);
    QString path = uri.toDisplayString(QUrl::RemoveFilename | QUrl::RemoveScheme).remove(QStringLiteral("//"));
    process->setWorkingDirectory(path);

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardOutput,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardOutput();
            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    QObject::connect
    (
        process,
        &QProcess::readyReadStandardError,
        this,
        [this, process]()
        {
            QByteArray processData = process->readAllStandardError();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    process->start(qsb, arguments);

    if(!process->waitForStarted(3000))
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Could not start shader compiler"));
    }

    if(!process->waitForFinished())
    {
        process->deleteLater();
        throw FileException(QStringLiteral("Shader compiler timeout"));
    }

    if(process->exitCode() != 0)
    {
        process->deleteLater();
        throw ShaderCompilerException(m_compilerOutput);
    }

    process->deleteLater();

    if(!QFile::remove(uri.toLocalFile()))
    {
        throw FileException(QStringLiteral("Could not clean up"));
    }
}

auto PackCompiler::removeDirectory(const QUrl &uri) -> bool
{
    QStringList arguments =
    {
        QStringLiteral("-rf"),
        uri.toLocalFile()
    };

    QProcess *process = new QProcess(this);

    process->start(QStringLiteral("rm"), arguments);

    if(!process->waitForStarted(3000))
    {
        process->deleteLater();
        return false;
    }

    if(!process->waitForFinished())
    {
        process->deleteLater();
        return false;
    }

    if(process->exitCode() != 0)
    {
        process->deleteLater();
        return false;
    }

    process->deleteLater();
    return true;
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