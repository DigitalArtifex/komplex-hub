#include "downloadmanager.h"
#include "common/coreservices.h"
#include "common/exceptions.h"
#include "common/logging.h"
#include "common/shaderpackmetadata.h"

DownloadManager::DownloadManager(QObject *parent)
    : QObject{parent}
{

}

auto DownloadManager::downloadImage(const QString &author, const QString &authorId, const QString &description, const QUrl &url) noexcept(false) -> void
{
    QFuture future = QtConcurrent::run
    (
        [this, url, author, authorId, description]()
        {
            QNetworkRequest request(url);
            QUrl localUri;
            QString id = QUuid::createUuidV7().toString(QUuid::WithoutBraces);

            try
            {
                QUrl downloadUri = download(request, id);

                ShaderPackMetadata metadata;
                metadata.setAuthor(author);
                metadata.setDescription(description);
                metadata.setName(QStringLiteral("Pexels Image (%1)").arg(id));

                setState(Complete);
                Q_EMIT downloadComplete(downloadUri.toString());
            }
            catch (FileException e)
            {
                setError(QStringLiteral("File Exception"), e.message);
                localUri.clear();
            }
            catch (NetworkException e)
            {
                setError(QStringLiteral("Network Exception"), e.message);
                localUri.clear();
            }
        }
    );
}

auto DownloadManager::downloadPack(const QString &id) noexcept(false) -> void
{
    QFuture future = QtConcurrent::run
    (
        [this, id]()
        {
            QUrl uri = QUrl
            (
                QStringLiteral("%1/%2/%3").arg
                (
                    KOMPLEX_API_HOST,
                    KOMPLEX_API_VERSION,
                    KOMPLEX_ENDPOINT_PACKS_ITEM,
                    id
                )
            );

            QNetworkRequest request(uri);
            request.setRawHeader(QByteArray("uuid"), id.toUtf8());

            QUrl localUri;

            try
            {
                QUrl tempUri = download(request, id);
                QUrl decompressedUri = decompress(tempUri);

                ShaderToyEntry entry = readShaderToyEntry(decompressedUri);
                //scan and replace media via user choice. resave with decompressedUri

                QUrl compiledUri = compile(decompressedUri);
                QUrl installedUri = install(compiledUri);

                setState(Complete);

                Q_EMIT downloadComplete(installedUri.toString());
            }
            catch (FileException e)
            {
                setError(QStringLiteral("File Exception"), e.message);
                localUri.clear();
            }
            catch (NetworkException e)
            {
                setError(QStringLiteral("Network Exception"), e.message);
                localUri.clear();
            }
        }
    );
}

auto DownloadManager::reset() -> void
{
    setError(QString(), QString());
    setState(Idle);
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
}

auto DownloadManager::compile(const QUrl &uri) noexcept(false) -> QUrl
{
    if(!uri.isLocalFile())
    {
        throw FileException(QStringLiteral("Uri needs to be a local file"), 0);
    }

    setState(Compiling);

    QDir localToolsDirectory
    (
        QStringLiteral("%1/.local/share/komplex/tools").arg
        (
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
        )
    );

    QDir outputDirectory = QStringLiteral("%1/komplex/build").arg
    (
        QStandardPaths::writableLocation(QStandardPaths::TempLocation)
    );

    QStringList arguments =
    {
        localToolsDirectory.absoluteFilePath(QStringLiteral("stc.py")),
        QStringLiteral("-i"),
        uri.toLocalFile(),
        QStringLiteral("-o"),
        outputDirectory.absolutePath()
    };

    if(!QFile::exists(localToolsDirectory.absoluteFilePath(QStringLiteral("stc.py"))))
    {
        throw FileException
        (
            QStringLiteral("Shader Compiler is not installed at %1").arg
            (
                localToolsDirectory.absoluteFilePath(QStringLiteral("stc.py"))
            )
        );
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

    process->start(QStringLiteral("python3"), arguments);

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
        throw FileException(QStringLiteral("Shader compiler error"));
    }

    process->deleteLater();

    QUrl outputUri = outputDirectory.absolutePath();
    outputUri.setScheme(QStringLiteral("file://"));

    return outputUri;
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

auto DownloadManager::decompress(const QUrl &uri) noexcept(false) -> QUrl
{
    return {};
}

// void DownloadManager::save(ShaderToyEntry entry)
// {
//     QString directoryLocation = QStringLiteral("%1/komplex/src/%2").arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation), entry.metadata.id);
//     QDir directory(directoryLocation);

//     if(!directory.exists())
//     {
//         directory.mkpath(directoryLocation + QStringLiteral("/shaders"));
//         directory.mkpath(directoryLocation + QStringLiteral("/images"));
//         directory.mkpath(directoryLocation + QStringLiteral("/videos"));
//     }

//     QDir shaderDirectory(directoryLocation + QStringLiteral("/shaders"));
//     QDir imageDirectory(directoryLocation + QStringLiteral("/images"));
//     // QDir videoDirectory(directoryLocation + QStringLiteral("/videos"));

//     QJsonObject rootObject;
//     rootObject[QStringLiteral("author")] = entry.metadata.username;
//     rootObject[QStringLiteral("name")] = entry.metadata.name;
//     rootObject[QStringLiteral("version")] = entry.metadata.version;
//     rootObject[QStringLiteral("engine")] = QStringLiteral("shadertoy");
//     rootObject[QStringLiteral("description")] = entry.metadata.description;
//     rootObject[QStringLiteral("id")] = entry.metadata.id;
//     rootObject[QStringLiteral("tags")] = QJsonArray::fromStringList(entry.metadata.tags);
//     QMap<QString,QString> externalMedia;

//     externalMedia.insert
//     (
//         directory.absoluteFilePath(QStringLiteral("thumbnail.jpg")),
//         QStringLiteral("/media/shaders/%1.jpg").arg(entry.metadata.id)
//     );

//     for(const ShaderToyRenderPass &pass : std::as_const(entry.renderPasses))
//     {
//         // skip tone generators
//         if(pass.type == QStringLiteral("sound"))
//             continue;

//         QString passName = pass.name;

//         if(passName.contains(QStringLiteral("Buf")) && !passName.contains(QStringLiteral("Buffer")))
//             passName.replace(QStringLiteral("Buf"), QStringLiteral("Buffer"));

//         QFile shaderFile(shaderDirectory.absoluteFilePath(passName + QStringLiteral(".frag")));

//         if(!shaderFile.open(QFile::WriteOnly))
//         {
//             qWarning() << QStringLiteral("Could not open shader file for saving");
//             return;
//         }

//         if(shaderFile.write(pass.code) != pass.code.length())
//         {
//             qWarning() << QStringLiteral("Could not write shader file data");
//             shaderFile.close();
//             return;
//         }

//         shaderFile.close();

//         //this is the common file
//         if(pass.type == QStringLiteral("common"))
//             continue; // wont have any inputs

//         const ShaderToyRenderOutput *channelOutput = nullptr;

//         for(const ShaderToyRenderOutput &output : std::as_const(pass.outputs))
//         {
//             if(output.channel == 0)
//             {
//                 channelOutput = &output;
//                 break;
//             }
//         }

//         QList<QJsonObject> channels(4);

//         QJsonObject *passObject = nullptr;

//         //this is the root shader
//         if(pass.type == QStringLiteral("image"))
//         {
//             rootObject[QStringLiteral("source")] = QStringLiteral("./shaders/%1.frag.qsb").arg(pass.name);
//             passObject = &rootObject;
//         }
//         else
//             passObject = new QJsonObject;

//         for(const ShaderToyRenderInput &input : std::as_const(pass.inputs))
//         {
//             /*
//             * Only recursive buffers, images, videos and shader buffers are currently supported.
//             * audio will default to audio capture
//             */

//             if(!m_supportedChannelTypes.contains(input.ctype))
//             {
//                 qWarning() << input.ctype << QStringLiteral(" is not a valid channel type");
//                 continue;
//             }

//             // recursive buffer reference
//             if(channelOutput && input.id == channelOutput->id)
//             {
//                 passObject->insert(QStringLiteral("frame_buffer_channel"), input.channel);
//                 continue;
//             }

//             if(input.ctype == QStringLiteral("buffer"))
//             {
//                 // get input reference by id
//                 const ShaderToyRenderPass *inputPass = nullptr;

//                 for(const ShaderToyRenderPass &passSubScan : std::as_const(entry.renderPasses))
//                 {
//                     for(const ShaderToyRenderOutput &output : std::as_const(passSubScan.outputs))
//                     {
//                         if(output.id == input.id && output.channel == 0)
//                         {
//                             inputPass = &passSubScan;
//                             break;
//                         }

//                         if(inputPass)
//                             break;
//                     }
//                 }

//                 //whoopsie
//                 if(!inputPass)
//                     continue;

//                 QString name = inputPass->name.toCaseFolded();
//                 name.replace(name.length() - 1, 1, name.right(1).toUpper());
//                 name.remove(QLatin1Char(' '));
//                 name.replace(QStringLiteral("buf"), QStringLiteral("buffer"));

//                 channels[input.channel][QStringLiteral("source")] = QStringLiteral("{%1}").arg(name);
//             }

//             else if(input.ctype == QStringLiteral("audio"))
//                 channels[input.channel][QStringLiteral("type")] = 4;

//             else if(input.ctype == QStringLiteral("texture"))
//             {
//                 QString filename = input.source;
//                 filename = filename.mid(filename.lastIndexOf(QLatin1Char('/')) + 1);

//                 channels[input.channel][QStringLiteral("type")] = 0;
//                 channels[input.channel][QStringLiteral("source")] = QStringLiteral("./images/%1").arg(filename);

//                 externalMedia.insert(imageDirectory.absoluteFilePath(filename), input.source);
//             }

//             //select video file after compilation
//             else if(input.ctype == QStringLiteral("video"))
//             {
//                 //set the channel source to a uuid then add that uuid to the video
//                 // selection stringlist
//                 QString sourceName = QUuid::createUuidV7().toString();
//                 channels[input.channel][QStringLiteral("type")] = 1;
//                 channels[input.channel][QStringLiteral("source")] = sourceName;

//                 QStringList newSelections = m_videoSelections;
//                 newSelections += sourceName;

//                 setVideoSelections(newSelections);
//             }

//             channels[input.channel][QStringLiteral("filter")] = input.filter;
//             channels[input.channel][QStringLiteral("wrap")] = input.wrap;
//             channels[input.channel][QStringLiteral("invert")] = input.verticalFlip;
//             channels[input.channel][QStringLiteral("srgb")] = input.srgb;
//             channels[input.channel][QStringLiteral("internal")] = input.internal;
//         }

//         for(int i = 0; i < 4; ++i)
//         {
//             if(channels[i].isEmpty())
//                 continue;

//             passObject->insert(QStringLiteral("channel%1").arg(i), channels[i]);
//         }

//         //this is a buffer
//         if(pass.type == QStringLiteral("buffer"))
//         {
//             QString name = pass.name.toCaseFolded();
//             name.replace(name.length() - 1, 1, name.right(1).toUpper());
//             name.remove(QLatin1Char(' '));
//             name.replace(QStringLiteral("buf"), QStringLiteral("buffer"));
//             passObject->insert(QStringLiteral("source"), QStringLiteral("./shaders/%1.frag.qsb").arg(passName));

//             rootObject[name] = *passObject;
//         }

//         if(*passObject != rootObject)
//             delete passObject;
//     }

//     QFile shaderPackFile(directory.absoluteFilePath(QStringLiteral("pack.json")));

//     if(!shaderPackFile.open(QFile::WriteOnly))
//     {
//         qWarning() << QStringLiteral("Could not open pack file");
//         return;
//     }

//     QJsonDocument packDocument;
//     packDocument.setObject(rootObject);

//     QByteArray jsonData = packDocument.toJson(QJsonDocument::Indented);

//     if(shaderPackFile.write(jsonData) != jsonData.length())
//     {
//         qWarning() << QStringLiteral("Could not write pack data");
//         return;
//     }

//     const QStringList keys = externalMedia.keys();

//     // qWarning() << QStringLiteral("Downloading %1 Images").arg(externalMedia.count());
//     // setStatus(Compiling, QStringLiteral("Downloading images"));

//     setTotalDownloads(externalMedia.count());

//     for(const QString &key : keys)
//         downloadMedia(key, externalMedia[key]);
// }

auto DownloadManager::install(const QUrl &uri) noexcept(false) -> QUrl
{
    if(!uri.isLocalFile())
    {
        throw FileException(QStringLiteral("URI is not a local file"));
    }

    QDir tempDirectory(uri.toLocalFile());

    QString installLocation = QStringLiteral("%1/.local/share/komplex/packs/%2").arg
    (
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
        tempDirectory.dirName()
    );

    QDir installDirectory(installLocation);

    if(installDirectory.exists())
        installDirectory.removeRecursively();

    QProcess process;

    QObject::connect
    (
        &process,
        &QProcess::readyReadStandardOutput,
        this,
        [this, &process]()
        {
            QByteArray processData = process.readAllStandardOutput();

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
        &process,
        &QProcess::readyReadStandardError,
        this,
        [this, &process]()
        {
            QByteArray processData = process.readAllStandardError();

            if(!processData.isValidUtf8())
            {
                qWarning() << QStringLiteral("Process output not valid UTF8 data");
                return;
            }

            setCompilerOutput(m_compilerOutput + processData);
        }
    );

    QStringList arguments = QStringList
    {
        QStringLiteral("-R"),
        uri.toLocalFile(),
        installLocation
    };

    process.start(QStringLiteral("cp"), arguments);

    if(!process.waitForStarted(3000))
    {
        qWarning() << QStringLiteral("Could not start copy process: %1").arg(process.readAllStandardError());
        throw FileException(QStringLiteral("Could not start install process"));
    }

    if(!process.waitForFinished())
    {
        qWarning() << QStringLiteral("Copy process took longer than expected (>30s)");
        throw FileException(QStringLiteral("Install process took longer than expected (>30s)"));
    }

    QUrl installUri(installLocation);
    installUri.setScheme(QStringLiteral("file://"));

    return installUri;
}

auto DownloadManager::download(const QNetworkRequest &request, const QString &id) noexcept(false) -> QUrl
{
    QEventLoop loop;

    QUrl downloadUri = QStringLiteral("%1/%2").arg
    (
        QStandardPaths::writableLocation(QStandardPaths::TempLocation),
        request.url().path().split('/', Qt::SkipEmptyParts).last()
    );

    QWeakPointer<QNetworkAccessManager> reference = CoreServices::networkAccessManager();
    QSharedPointer<QNetworkAccessManager> manager = reference.toStrongRef();

    if(manager == nullptr)
    {
        throw NetworkException
        (
            QStringLiteral("Network Manager reference has already been deleted")
        );
    }

    QFile downloadFile(downloadUri.toLocalFile());

    if(!downloadFile.open(QFile::ReadWrite))
    {
        throw FileException(QStringLiteral("Could not open temp file location"));
    }

    QNetworkReply *reply = manager->post(request, nullptr);

    QObject::connect
    (
        reply,
        &QNetworkReply::finished,
        &loop,
        &QEventLoop::quit
    );

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
            setDownloadProgress(static_cast<qreal>(bytesDownloaded) / bytesTotal);
        }
    );

    QObject::connect
    (
        reply,
        &QNetworkReply::readyRead,
        this,
        [this, &downloadFile, &reply]()
        {
            if(!downloadFile.isOpen())
            {
                return;
            }

            quint64 bytes = reply->bytesAvailable();
            quint64 bytesWritten = downloadFile.write(reply->read(bytes));

            if(bytesWritten != bytes)
            {
                throw FileException(QStringLiteral("Could not write temp file data"));
            }
        }
    );

    if(!reply->isFinished())
    {
        loop.exec();
    }

    if(reply->bytesAvailable() > 0)
    {
        quint64 bytes = reply->bytesAvailable();
        quint64 bytesWritten = downloadFile.write(reply->read(bytes));

        if(bytesWritten != bytes)
        {
            throw FileException(QStringLiteral("Could not write temp file data"));
        }
    }

    downloadFile.close();
    manager.clear();

    return downloadUri;
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