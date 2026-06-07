#include "installedpackmanager.h"

InstalledPackManager::InstalledPackManager(QObject *parent)
    : QAbstractListModel{parent}
{
    m_installDirectoryUri = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    m_installDirectoryUri += QString("/.local/share/komplex/packs");

    // load in wallpapers on init
    rescan();
}

auto InstalledPackManager::rowCount(const QModelIndex &parent) const -> int
{
    Q_UNUSED(parent)
    return m_installedWallpapers.size();
}

auto InstalledPackManager::data(const QModelIndex &index, int role) const -> QVariant
{
    if(index.row() < 0 || index.row() >= m_installedWallpapers.count())
    {
        return QVariant();
    }

    QVariant data;

    switch(static_cast<DataRole>(role))
    {
    case UriRole:
        data = m_installedWallpapers[index.row()].uri;
        break;
    case AuthorRole:
        data = m_installedWallpapers[index.row()].author;
        break;
    case DescriptionRole:
        data = m_installedWallpapers[index.row()].description;
        break;
    case NameRole:
        data = m_installedWallpapers[index.row()].name;
        break;
    case ThumbnailRole:
        data = m_installedWallpapers[index.row()].thumbnail;
        break;
    case DateRole:
        data = m_installedWallpapers[index.row()].installDate;
        break;
    }

    return data;
}

auto InstalledPackManager::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)
    return createIndex(row, column, &m_installedWallpapers.at(row));
}

auto InstalledPackManager::columnCount(const QModelIndex &parent) const -> int
{
    Q_UNUSED(parent)
    return 0;
}

auto InstalledPackManager::parent(const QModelIndex &index) const -> QModelIndex
{
    Q_UNUSED(index)
    return QModelIndex();
}

auto InstalledPackManager::setData(const QModelIndex &index, const QVariant &value, int role) -> bool
{
    if(index.row() < 0 || index.row() >= m_installedWallpapers.count())
    {
        return false;
    }

    switch(static_cast<DataRole>(role))
    {
    case UriRole:
        m_installedWallpapers[index.row()].uri = value.toString();
        break;
    case AuthorRole:
        m_installedWallpapers[index.row()].author = value.toString();
        break;
    case DescriptionRole:
        m_installedWallpapers[index.row()].description = value.toString();
        break;
    case NameRole:
        m_installedWallpapers[index.row()].name = value.toString();
        break;
    case ThumbnailRole:
        m_installedWallpapers[index.row()].thumbnail = value.toString();
        break;
    case DateRole:
        return false;
        break;
    }

    return true;
}

auto InstalledPackManager::installedWallpapers() const -> QList<WallpaperInstallData>
{
    return m_installedWallpapers;
}

auto InstalledPackManager::resetInstalledWallpapers() -> void
{
    m_installedWallpapers.clear();
    Q_EMIT installedWallpapersChanged();
}

auto InstalledPackManager::rescan() -> void
{
    setState(Loading);
    m_installedWallpapers.clear();

    QDir installDirectory(m_installDirectoryUri);
    QStringList packLocations = installDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(const QString &packLocation : std::as_const(packLocations))
    {
        QDir packDirectory(installDirectory.absoluteFilePath(packLocation));

        if(!packDirectory.exists() || !packDirectory.exists(QString("pack.json")))
        {
            continue;
        }

        QString packFileLocation = packDirectory.absoluteFilePath(QString("pack.json"));
        QFile packFile(packFileLocation);

        if(!packFile.open(QFile::ReadOnly))
        {
            continue;
        }

        QByteArray packFileData = packFile.readAll();

        packFile.close();

        QJsonParseError documentError;
        QJsonDocument document = QJsonDocument::fromJson(packFileData, &documentError);

        if(documentError.error != QJsonParseError::NoError)
        {
            continue;
        }

        QJsonObject rootObject = document.object();

        if(!rootObject.contains(QString("author")) ||
            !rootObject.contains(QString("description")) ||
            !rootObject.contains(QString("name")))
        {
            continue;
        }

        QFileInfo packFileInfo(packFile);

        WallpaperInstallData data
        {
            packDirectory.absoluteFilePath(QString("pack.json")),
            rootObject["name"].toString(),
            rootObject["author"].toString(),
            rootObject["description"].toString(),
            QString("file://") + packDirectory.absoluteFilePath(QString("thumbnail.jpg")),
            packFileInfo.birthTime()
        };

        m_installedWallpapers.append(data);
    }

    Q_EMIT installedWallpapersChanged();
    setState(Idle);
}

auto InstalledPackManager::uninstall(QString uri) -> bool
{
    QUrl url(uri);
    url.setScheme(QString("file"));

    QFileInfo info(url.toLocalFile());
    QDir installDirectory = info.absoluteDir();

    QFileInfo directoryInfo(installDirectory.absolutePath());

    if(!directoryInfo.exists())
    {
        setErrorString(QString("Install directory does not exist"));
        return false;
    }

    // could not get QDir::rmdir to rm a symlink dir
    if(directoryInfo.isSymbolicLink())
    {
        QProcess process;
        QString command("unlink \"");
        command += url.toLocalFile() + QString("\"");

        process.startCommand(command);

        if(!process.waitForStarted())
        {
            setErrorString(QString("Process Failed To Start"));
            return false;
        }

        if(!process.waitForFinished())
        {
            setErrorString(QString("Process Timeout"));
            return false;
        }

        if(process.exitCode() != 0 || process.exitStatus() != QProcess::NormalExit)
        {
            setErrorString(process.readAllStandardError());
            return false;
        }

        return true;
    }

    if(!installDirectory.rmdir(installDirectory.absolutePath()))
    {
        setErrorString(QString("Install directory could not be removed"));
        return false;
    }

    return true;
}

auto InstalledPackManager::state() const -> InstalledPackManager::State
{
    return m_state;
}

auto InstalledPackManager::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto InstalledPackManager::resetState() -> void
{
    setState(Idle); // TODO: Adapt to use your actual default value
}

auto InstalledPackManager::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto InstalledPackManager::errorString() const -> QString
{
    return m_errorString;
}

auto InstalledPackManager::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

