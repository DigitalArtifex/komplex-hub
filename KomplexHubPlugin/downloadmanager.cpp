#include "downloadmanager.h"
#include "common/coreservices.h"
#include "common/exceptions.h"
#include "common/shaderpack.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{
    m_compiler = new PackCompiler(this);

    QObject::connect
    (
        m_compiler,
        &PackCompiler::progressChanged,
        this,
        &DownloadManager::compileProgressChanged
    );

    QObject::connect
    (
        m_compiler,
        &PackCompiler::currentStepChanged,
        this,
        &DownloadManager::compileStepsCompletedChanged
    );

    QObject::connect
    (
        m_compiler,
        &PackCompiler::totalStepsChanged,
        this,
        &DownloadManager::compileStepsChanged
    );

    QObject::connect
    (
        &m_moveProcess,
        &QProcess::readyReadStandardOutput,
        this,
        [this]()
        {
            QByteArray processData = m_moveProcess.readAllStandardOutput();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    QObject::connect
    (
        &m_moveProcess,
        &QProcess::readyReadStandardError,
        this,
        [this]()
        {
            QByteArray processData = m_moveProcess.readAllStandardError();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );
}

/**
 * [1] Download image from endpoint
 * [2] Apply generic single-media template, with an image source
 * [3] Optionally ask user if they would like to apply manipulative shader
 * [4] Install locally
 */
auto DownloadManager::downloadImage(const QString &author, const QString &authorId, const QString &description, const QUrl &url) -> void
{
    // QFuture future = QtConcurrent::run
    // (
    //     [this, url, author, authorId, description]()
    //     {
            QNetworkRequest request(url);
            QString requestUrl = request.url().toString();
            QString id = QUuid::createUuidV7().toString(QUuid::WithoutBraces);

            setState(Downloading);

            //QFuture<QUrl> downloadUri = download(request, id, Get);
            if(m_downloadFuture.isRunning())
            {
                m_downloadFuture.cancelChain();
            }

            m_downloadFuture = download(request, id, Get);

            m_downloadFuture
                .then
                (
                    [this, author, description, id](QUrl result) -> QUrl
                    {
                        setState(Installing);

                        ShaderPack metadata;
                        metadata.setAuthor(author);
                        metadata.setDescription(description);
                        metadata.setName(QStringLiteral("Pexels Image (%1)").arg(id));
                        metadata.setSource(QStringLiteral("./images/%1").arg(result.fileName()));

                        QUrl packUri
                        (
                            QStringLiteral("file://%1/%2").arg
                            (
                                QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                                id
                            )
                        );

                        QUrl packImageUri
                        (
                            QStringLiteral("file://%1/%2/images/%3").arg
                            (
                                QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                                id,
                                result.fileName()
                            )
                        );

                        QDir packDirectory(packUri.toLocalFile());

                        if(!packDirectory.mkpath(packUri.toLocalFile()))
                        {
                            throw FileException(QStringLiteral("Could not create directory"));
                        }

                        packDirectory.mkdir(QStringLiteral("images"));

                        QFile::rename(result.toLocalFile(), packImageUri.toLocalFile());

                        QFile packFile
                        (
                            packDirectory.absoluteFilePath
                            (
                                QStringLiteral("pack.json")
                            )
                        );

                        if(!packFile.open(QFile::ReadWrite))
                        {
                            setError(QStringLiteral("File Error"), packFile.errorString());
                            throw FileException(packFile.errorString());
                        }

                        QByteArray data = metadata.json().toJson(QJsonDocument::Indented);

                        if(packFile.write(data) != data.length())
                        {
                            setError(QStringLiteral("File Error"), packFile.errorString());
                            throw FileException(packFile.errorString());
                        }

                        packFile.close();

                        return packUri;
                    }
                )
                .then
                (
                    [this](QUrl result) -> QUrl
                    {
                        return install(result);
                    }
                )
                .then
                (
                    [this](QUrl result)
                    {
                        setLastInstalledFile(result.toLocalFile());
                        setState(Complete);
                    }
                )
                .onCanceled
                (
                    [this]()
                    {
                        reset();
                    }
                )
                .onFailed
                (
                    [this] (const NetworkException &e)
                    {
                        reset();
                        setError(QStringLiteral("Network Exception %1").arg(QString::number(e.errorCode)), e.message);
                    }
                )
                .onFailed
                (
                    [this] (const FileException &e)
                    {
                        reset();
                        setError(QStringLiteral("File Exception %1").arg(QString::number(e.errorCode)), e.message);
                    }
                )
                .onFailed
                (
                    [this] ()
                    {
                        reset();
                        setError(QStringLiteral("Unknown Exception"), QString());
                    }
                );

    //         downloadUri.waitForFinished();
    //     }
    // );
}

auto DownloadManager::downloadVideo(const QString &author, const QString &authorId, const QString &description, const QUrl &url) -> void
{
    QFuture future = QtConcurrent::run
    (
        [this, url, author, authorId, description]()
        {
            QNetworkRequest request(url);
            QString id = QUuid::createUuidV7().toString(QUuid::WithoutBraces);

            QFuture<QUrl> downloadUri = download(request, id);

            downloadUri
                .then
                (
                    [this, author, description, id](QUrl result) -> QUrl
                    {
                        ShaderPack metadata;
                        metadata.setAuthor(author);
                        metadata.setDescription(description);
                        metadata.setName(QStringLiteral("Pexels Video (%1)").arg(id));
                        metadata.setType(ShaderPack::Video);

                        QUrl packUri
                        (
                            QStringLiteral("%1/%2").arg
                            (
                                QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                                id
                            )
                        );

                        QUrl packImageUri
                        (
                            QStringLiteral("%1/%2/images/%3").arg
                            (
                                QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                                id,
                                result.fileName()
                            )
                        );

                        QDir packDirectory(packUri.toLocalFile());
                        packDirectory.mkpath(packUri.toLocalFile());
                        packDirectory.mkdir(QStringLiteral("images"));

                        QFile::rename(result.toLocalFile(), packImageUri.toLocalFile());

                        QFile packFile
                        (
                            packDirectory.absoluteFilePath
                            (
                                QStringLiteral("pack.json")
                            )
                        );

                        if(!packFile.open(QFile::ReadWrite))
                        {
                            setError(QStringLiteral("File Error"), packFile.errorString());
                            throw FileException(packFile.errorString());
                        }

                        QByteArray data = metadata.json().toJson(QJsonDocument::Compact);

                        if(packFile.write(data) != data.length())
                        {
                            setError(QStringLiteral("File Error"), packFile.errorString());
                            throw FileException(packFile.errorString());
                        }

                        packFile.close();

                        return packUri;
                    }
                )
                .then
                (
                    [this](QUrl result)
                    {
                        install(result);
                    }
                )
                .onCanceled
                (
                    [this]()
                    {
                        reset();
                    }
                )
                .onFailed
                (
                    [this] ()
                    {
                        reset();
                    }
                );
        }
    );
}

auto DownloadManager::downloadPack(const QString &id) -> void
{
    QFuture aether = QtConcurrent::run
    (
        [this, id]()
        {
            QUrl downloadUrl = QUrl
            (
                QStringLiteral("%1/%2/%3").arg
                (
                    KOMPLEX_API_HOST,
                    KOMPLEX_API_VERSION,
                    KOMPLEX_ENDPOINT_PACKS_ITEM
                )
            );

            QNetworkRequest request(downloadUrl);
            request.setRawHeader(QByteArray("uuid"), id.toUtf8());

            QFuture<QUrl> downloadUri = download(request, id, Post);

            downloadUri
                .then
                (
                    [this](QUrl result) -> QUrl
                    {
                        return m_compiler->process(result).result();
                    }
                )
                .then
                (
                    [this](QUrl result) -> QUrl
                    {
                        return install(result);
                    }
                )
                .then
                (
                    [this] (QUrl result)
                    {
                        setState(Complete);
                    }
                )
                .onFailed
                (
                    [this]()
                    {
                        reset();
                        setError(m_compiler->errorTitle(), m_compiler->errorMessage());
                    }
                )
                .onCanceled
                (
                    [this]()
                    {
                        reset();
                    }
                );
        }
    );
}

auto DownloadManager::reset() -> void
{
    setError(QString(), QString());
    setState(Idle);
}

auto DownloadManager::compileProgress() -> qreal
{
    if(m_compiler == nullptr)
    {
        return 0;
    }

    return m_compiler->progress();
}

auto DownloadManager::compileSteps() -> qint64
{
    if(m_compiler == nullptr)
    {
        return 0;
    }

    return m_compiler->totalSteps();
}

auto DownloadManager::compileStepsCompleted() -> qint64
{
    if(nullptr == m_compiler)
    {
        return 0;
    }

    return m_compiler->currentStep();
}

auto DownloadManager::setError(const QString &title, const QString &message) -> void
{
    if(title == m_errorTitle && message == m_errorMessage)
    {
        return;
    }

    m_errorMessage = std::move(message);
    m_errorTitle = std::move(title);
    Q_EMIT errorChanged();

    setState(Error);
}

void DownloadManager::setCompilerOutput(const QString &compilerOutput)
{
    if (m_compilerOutput == compilerOutput)
    {
        return;
    }

    m_compilerOutput = compilerOutput;
    emit compilerOutputChanged();
}

auto DownloadManager::setState(State state) -> void
{
    if(state == m_state)
    {
        return;
    }

    m_state = state;
    Q_EMIT stateChanged();

    return;
}

auto DownloadManager::setDownloadProgress(qreal progress) -> void
{
    if(qFuzzyCompare(progress, m_downloadProgress))
    {
        return;
    }

    m_downloadProgress = progress;
    Q_EMIT downloadProgressChanged();
}

auto DownloadManager::install(const QUrl &uri) noexcept(false) -> QUrl
{
    if(!uri.isLocalFile())
    {
        throw FileException(QStringLiteral("URI is not a local file"));
    }

    QDir tempDirectory(uri.toLocalFile());

    QUrl installLocation = QStringLiteral("%1/.local/share/komplex/packs/%2").arg
    (
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        tempDirectory.dirName()
    );

    QDir installDirectory(installLocation.toLocalFile());

    if(installDirectory.exists())
        installDirectory.removeRecursively();

    QStringList arguments = QStringList
    {
        uri.toLocalFile(),
        installLocation.toLocalFile()
    };

    m_moveProcess.start(QStringLiteral("mv"), arguments);

    if(!m_moveProcess.waitForStarted(3000))
    {
        qWarning() << QStringLiteral("Could not start copy process: %1").arg(m_moveProcess.readAllStandardError());
        throw FileException(QStringLiteral("Could not start install process"));
    }

    if(!m_moveProcess.waitForFinished())
    {
        qWarning() << QStringLiteral("Copy process took longer than expected (>30s)");
        throw FileException(QStringLiteral("Install process took longer than expected (>30s)"));
    }

    return installLocation;
}

auto DownloadManager::download(const QNetworkRequest &request, const QString &id, RequestType type) -> QFuture<QUrl>
{
    return QtConcurrent::run
    (
        [this, request, id, type]() -> QUrl
        {
            QEventLoop loop;
            QString filename = id;

            if(filename.isEmpty())
            {
                filename = QUuid::createUuidV7().toString();
            }

            QUrl downloadUri = QStringLiteral("file://%1/%2").arg
            (
                QStandardPaths::writableLocation(QStandardPaths::TempLocation),
                request.url().fileName()
            );

            auto manager = CoreServices::networkAccessManager().toStrongRef();

            if(manager == nullptr)
            {
                throw NetworkException
                {
                    QStringLiteral("Network Manager reference has already been deleted")
                };
            }

            QFile downloadFile(downloadUri.toLocalFile());

            if(downloadFile.exists() && !downloadFile.remove())
            {
                throw FileException(downloadFile.errorString());
            }

            QNetworkReply *reply = nullptr;

            switch(type)
            {
            case Post:
                reply = manager->post(request, nullptr);
                break;
            default:
            case Get:
                reply = manager->get(request, nullptr);
                break;
            }

            QObject::connect
            (
                reply,
                &QNetworkReply::errorOccurred,
                &loop,
                [&loop](QNetworkReply::NetworkError error) -> void
                {
                    loop.quit();

                    throw NetworkException
                    (
                        QStringLiteral("Network Error %1").arg
                        (
                            QString::number(static_cast<qint64>(error))
                        )
                    );
                }
            );

            QObject::connect
            (
                reply,
                &QNetworkReply::downloadProgress,
                this,
                [this](qint64 bytesDownloaded, qint64 bytesTotal)
                {
                    setDownloadSize(bytesTotal);
                    setDownloadedBytes(bytesDownloaded);
                    setDownloadProgress(static_cast<qreal>(bytesDownloaded) / bytesTotal);
                }
            );

            QObject::connect
            (
                reply,
                &QNetworkReply::readyRead,
                this,
                [this, downloadUri, reply]()
                {
                    QFile downloadFile(downloadUri.toLocalFile());

                    if(!downloadFile.open(QFile::ReadWrite | QFile::Append))
                    {
                        throw FileException(QStringLiteral("Could not open temp file location"));
                    }

                    quint64 bytes = reply->bytesAvailable();
                    quint64 bytesWritten = downloadFile.write(reply->readAll()/*bytes)*/);

                    downloadFile.close();
                }
            );

            QObject::connect
            (
                reply,
                &QNetworkReply::finished,
                &loop,
                &QEventLoop::quit
            );

            if(!reply->isFinished())
            {
                loop.exec();
            }

            manager.clear();

            return downloadUri;
        }
    );
}

auto DownloadManager::readShaderToyEntry(const QUrl &uri) noexcept(false) -> ShaderToyEntry
{
    if(!uri.isLocalFile())
    {
        throw FileException(QStringLiteral("URI is not a local file"));
    }

    QDir packDir(uri.toLocalFile());

    if(!packDir.exists(QStringLiteral("pack.json")))
    {
        throw FileException(QStringLiteral("Pack file not found"));
    }

    QFile packFile(packDir.absoluteFilePath(QStringLiteral("pack.json")));

    if(!packFile.open(QFile::ReadOnly))
    {
        throw FileException
        (
            QStringLiteral("Could not open pack file: %1").arg
            (
                packFile.errorString()
            )
        );
    }

    QByteArray data = packFile.readAll();
    QJsonParseError jsonError;

    QJsonDocument document = QJsonDocument::fromJson(data, &jsonError);

    if(jsonError.error != QJsonParseError::NoError)
    {
        throw FileException
        (
            QStringLiteral("Could parse pack file: %1").arg
            (
                jsonError.errorString()
            )
        );
    }

    QJsonObject documentObject = document.object();
    QJsonObject rootObject = documentObject[QStringLiteral("Shader")].toObject();
    QJsonObject infoObject = rootObject[QStringLiteral("info")].toObject();
    QJsonArray tagsArray = infoObject[QStringLiteral("tags")].toArray();

    QStringList tags;

    for(const QJsonValue &tag : std::as_const(tagsArray))
        tags += tag.toString();

    ShaderToyEntry entry;

    entry.metadata = ShaderToyMetadata
    {
        QDateTime::fromSecsSinceEpoch(infoObject[QStringLiteral("date")].toInt()),
        infoObject[QStringLiteral("description")].toString(),
        static_cast<quint64>(infoObject[QStringLiteral("flags")].toInt()),
        static_cast<bool>(infoObject[QStringLiteral("hasLiked")].toInt()),
        infoObject[QStringLiteral("id")].toString(),
        static_cast<quint64>(infoObject[QStringLiteral("likes")].toInt()),
        infoObject[QStringLiteral("name")].toString(),
        static_cast<quint64>(infoObject[QStringLiteral("published")].toInt()),
        tags,
        static_cast<bool>(infoObject[QStringLiteral("usePreview")].toInt()),
        infoObject[QStringLiteral("username")].toString(),
        rootObject[QStringLiteral("ver")].toString(),
        static_cast<quint64>(infoObject[QStringLiteral("views")].toInt())
    };

    QJsonArray ShaderToyRenderPassArray = rootObject[QStringLiteral("renderpass")].toArray();

    for(const QJsonValue &ShaderToyRenderPassValue : std::as_const(ShaderToyRenderPassArray))
    {
        QJsonArray inputArray = ShaderToyRenderPassValue[QStringLiteral("inputs")].toArray();
        QJsonArray outputArray = ShaderToyRenderPassValue[QStringLiteral("outputs")].toArray();
        QList<ShaderToyRenderInput> inputs;
        QList<ShaderToyRenderOutput> outputs;

        for(const QJsonValue &inputValue : std::as_const(inputArray))
        {
            QJsonObject samplerObject = inputValue[QStringLiteral("sampler")].toObject();
            inputs.append
            (
                ShaderToyRenderInput
                {
                    static_cast<quint8>(inputValue[QStringLiteral("channel")].toInt()),
                    inputValue[QStringLiteral("ctype")].toString(),
                    samplerObject[QStringLiteral("filter")].toString(),
                    static_cast<quint64>(inputValue[QStringLiteral("id")].toInt()),
                    samplerObject[QStringLiteral("internal")].toString(),
                    static_cast<bool>(inputValue[QStringLiteral("published")].toInt()),
                    inputValue[QStringLiteral("src")].toString(),
                    samplerObject[QStringLiteral("srgb")].toBool(),
                    samplerObject[QStringLiteral("verticalFlip")].toBool(),
                    samplerObject[QStringLiteral("wrap")].toString()
                }
            );
        }

        for(const QJsonValue &outputValue : std::as_const(outputArray))
        {
            outputs.append
            (
                ShaderToyRenderOutput
                {
                    static_cast<quint8>(outputValue[QStringLiteral("channel")].toInt()),
                    static_cast<quint64>(outputValue[QStringLiteral("id")].toInt())
                }
            );
        }

        entry.renderPasses.append
        (
            ShaderToyRenderPass
            {
                ShaderToyRenderPassValue[QStringLiteral("code")].toVariant().toByteArray(),
                ShaderToyRenderPassValue[QStringLiteral("description")].toString(),
                inputs,
                ShaderToyRenderPassValue[QStringLiteral("name")].toString(),
                outputs,
                ShaderToyRenderPassValue[QStringLiteral("type")].toString()
            }
        );
    }

    return std::move(entry);
}

auto DownloadManager::lastInstalledFile() const -> const QString &
{
    return m_lastInstalledFile;
}

auto DownloadManager::setLastInstalledFile(const QString &lastInstalledFile) -> void
{
    if (m_lastInstalledFile == lastInstalledFile)
        return;
    m_lastInstalledFile = lastInstalledFile;
    emit lastInstalledFileChanged();
}

auto DownloadManager::setDownloadedBytes(qint64 downloadedBytes) -> void
{
    if (m_downloadedBytes == downloadedBytes)
        return;

    m_downloadedBytes = downloadedBytes;
    emit downloadedBytesChanged();
}

auto DownloadManager::setDownloadSize(qint64 downloadSize) -> void
{
    if (m_downloadSize == downloadSize)
        return;

    m_downloadSize = downloadSize;
    emit downloadSizeChanged();
}