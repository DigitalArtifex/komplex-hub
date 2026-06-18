#include "settingsmanager.h"
#include <qassert.h>
#include <qcontainerfwd.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonparseerror.h>
#include <qprocess.h>
#include <qstringview.h>

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    m_homeLocation = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    m_shaderPackLocation = m_homeLocation + QString("/.local/share/komplex/packs");
    m_configLocation = m_homeLocation + QString("/.config/plasma-org.kde.plasma.desktop-appletsrc");

    // load available shader packs
    QDir shaderPackDirectory(m_shaderPackLocation);

    if(shaderPackDirectory.exists())
    {
        QStringList shaderPackDirectoryList = shaderPackDirectory.entryList(
            QDir::Dirs | QDir::NoDotAndDotDot
        );

        for(const QString &shaderPackDirectoryListing : std::as_const(shaderPackDirectoryList))
        {
            QDir installedShaderPackDirectory(shaderPackDirectory.absoluteFilePath(shaderPackDirectoryListing));

            if(!installedShaderPackDirectory.exists("pack.json"))
            {
                continue;
            }

            QString uri = installedShaderPackDirectory.absoluteFilePath(QString("pack.json"));
            QString name = packNameFromUri(uri);

            if(name.isNull() || name.isEmpty())
            {
                continue;
            }

            WallpaperLocationData metadata {
                .name = name,
                .uri = installedShaderPackDirectory.absoluteFilePath("pack.json")
            };

            m_availableShaderPacksMetadata.append(metadata);
        }
    }

    m_shaderPackIndex = indexOfShaderPack();
}

SettingsManager::~SettingsManager()
{

}

auto SettingsManager::get(const QString &key) -> QString
{
    QProcess process;
    QString command("kreadconfig6");
    QString result = QString();

    command = QString("kreadconfig6 --file ");
    command += homeLocation() + QString("/.config/plasma-org.kde.plasma.desktop-appletsrc");
    command += QString(" --group \"Containments\" --group \"25\" --group \"Wallpaper\"");
    command += QString(" --group \"com.github.digitalartifex.komplex\" --group \"General\" --key");
    command += QString(" \"") + key + QString("\"");

    process.startCommand(command);

    if(!process.waitForStarted())
    {
        Q_EMIT error(QString("Process Failed To Start"), process.errorString());
    }

    if(!process.waitForFinished())
    {
        Q_EMIT error(QString("Process Timeout"), process.errorString());
    }

    if(process.exitCode() == 0 && process.exitStatus() == QProcess::NormalExit)
    {
        result = process.readAllStandardOutput();

        if(result.endsWith(QString("\n")))
        {
            result.removeLast();
        }
    }
    else
    {
        QString error(process.readAll());
        Q_EMIT this->error(QString("Read Error"), error);
    }

    return result;
}

auto SettingsManager::set(const QString &key, const QVariant &value) -> bool
{
    // construct the command to kwriteconfig6
    QString command = QString("kwriteconfig6 --file ");
    command += homeLocation() + QString("/.config/plasma-org.kde.plasma.desktop-appletsrc");
    command += QString(" --group \"Containments\" --group \"25\" --group \"Wallpaper\"");
    command += QString(" --group \"com.github.digitalartifex.komplex\" --group \"General\" --key");
    command += QString(" \"") + key + QString("\" ") + value.toString();

    // start the process
    QProcess process;
    process.startCommand(command);

    // throw error if failed to start
    if(!process.waitForStarted())
    {
        Q_EMIT error(QString("Process Failed To Start"), process.errorString());
        return false;
    }

    // throw error if there was a timeout
    if(!process.waitForFinished())
    {
        Q_EMIT error(QString("Process Timeout"), process.errorString());
        return false;
    }

    // throw error if there was an ERRNO or issue running the process
    if(process.exitCode() != 0 || process.exitStatus() != QProcess::NormalExit)
    {
        Q_EMIT this->error(QString("Write Error"), process.readAllStandardError());
        return false;
    }

    // success
    return true;
}

auto SettingsManager::shaderPack() -> QString
{
    m_shaderPack = get(QString("shader_package"));

    //update pack name
    QString packName = packNameFromUri(m_shaderPack);
    setShaderPackName(packName);

    return m_shaderPack;
}

auto SettingsManager::setShaderPack(const QString &uri) -> void
{
    QString packName = packNameFromUri(uri);

    if(uri != m_shaderPack)
    {
        if(!set(QString("shader_package"), uri))
        {
            return;
        }

        m_shaderPack = uri;
        Q_EMIT shaderPackChanged();
    }

    setShaderPackName(packName);
}

auto SettingsManager::shaderPackName() const -> QString
{
    return m_shaderPackName;
}

auto SettingsManager::availableShaderPacks() -> QStringList
{
    QStringList availableShaderPacks;

    for(const WallpaperLocationData &data : std::as_const(m_availableShaderPacksMetadata))
    {
        availableShaderPacks.append(data.name);
    }

    return availableShaderPacks;
}

auto SettingsManager::homeLocation() const -> QString
{
    return m_homeLocation;
}

auto SettingsManager::configLocation() const -> QString
{
    return m_configLocation;
}

auto SettingsManager::shaderPackLocation() const -> QString
{
    return m_shaderPackLocation;
}

auto SettingsManager::shaderPackIndex() const -> qint64
{
    return m_shaderPackIndex;
}

auto SettingsManager::setShaderPackIndex(qint64 shaderPackIndex) -> void
{
    if (m_shaderPackIndex == shaderPackIndex)
    {
        return;
    }

    m_shaderPackIndex = shaderPackIndex;
    emit shaderPackIndexChanged();
}

auto SettingsManager::setShaderPackName(const QString &shaderPackName) -> void
{
    if (m_shaderPackName == shaderPackName)
    {
        return;
    }

    m_shaderPackName = shaderPackName;
    emit shaderPackNameChanged();
}

auto SettingsManager::resolutionX() -> quint64
{
    QString value = get(QString("resolution_x"));

    if(value.isNull() || value.isEmpty())
    {
        setResolutionX(1920);
    }
    else
    {
        quint64 convertedValue = value.toULongLong();

        if(convertedValue != m_resolutionX)
        {
            m_resolutionX = convertedValue;
            Q_EMIT resolutionXChanged();
        }
    }

    return m_resolutionX;
}

auto SettingsManager::setResolutionX(quint64 resolutionX) -> void
{
    if (m_resolutionX == resolutionX)
    {
        return;
    }

    if(!set(QString("resolution_x"), resolutionX))
    {
        return;
    }

    m_resolutionX = resolutionX;
    emit resolutionXChanged();
}

auto SettingsManager::resolutionY() -> quint64
{
    QString value = get(QString("resolution_y"));

    if(value.isNull() || value.isEmpty())
    {
        setResolutionY(1080);
    }
    else
    {
        quint64 convertedValue = value.toULongLong();

        if(convertedValue != m_resolutionY)
        {
            m_resolutionY = convertedValue;
            Q_EMIT resolutionYChanged();
        }
    }

    return m_resolutionY;
}

auto SettingsManager::setResolutionY(quint64 resolutionY) -> void
{
    if (m_resolutionY == resolutionY)
    {
        return;
    }

    if(!set(QString("resolution_y"), resolutionY))
    {
        return;
    }

    m_resolutionY = resolutionY;
    emit resolutionYChanged();
}

auto SettingsManager::targetFramerate() -> quint64
{
    QString value = get(QString("framerate"));

    if(value.isNull() || value.isEmpty())
    {
        setTargetFramerate(60);
    }
    else
    {
        bool okay = false;
        quint64 convertedValue = value.toULongLong(&okay);

        if(!okay)
        {
            setTargetFramerate(60);
        }

        if(convertedValue != m_targetFramerate)
        {
            m_targetFramerate = convertedValue;
            Q_EMIT targetFramerateChanged();
        }
    }

    return m_targetFramerate;
}

auto SettingsManager::setTargetFramerate(quint64 targetFramerate) -> void
{
    if (m_targetFramerate == targetFramerate)
        return;

    if(!set(QString("framerate"), targetFramerate))
        return;

    m_targetFramerate = targetFramerate;
    emit targetFramerateChanged();
}

auto SettingsManager::shaderSpeed() -> qreal
{
    QString value = get(QString("shaderSpeed"));

    if(value.isNull() || value.isEmpty())
    {
        setShaderSpeed(1.0);
    }
    else
    {
        bool okay = false;
        qreal convertedValue = value.toFloat(&okay);

        if(!okay)
            setShaderSpeed(1.0);

        if(convertedValue != m_shaderSpeed)
        {
            m_shaderSpeed = convertedValue;
            Q_EMIT shaderSpeedChanged();
        }
    }

    return m_shaderSpeed;
}

auto SettingsManager::setShaderSpeed(qreal shaderSpeed) -> void
{
    if (qFuzzyCompare(m_shaderSpeed, shaderSpeed))
    {
        return;
    }

    if(!set(QString("shaderSpeed"), shaderSpeed))
    {
        return;
    }

    m_shaderSpeed = shaderSpeed;
    emit shaderSpeedChanged();
}

auto SettingsManager::mouseTrackingEnabled() -> bool
{
    QString value = get(QString("mouseAllowed"));

    if(value.isNull() || value.isEmpty())
    {
        setTargetFramerate(60);
    }
    else
    {
        bool convertedValue = value.trimmed().compare("true", Qt::CaseInsensitive);

        if(convertedValue != m_mouseTrackingEnabled)
        {
            m_mouseTrackingEnabled = convertedValue;
            Q_EMIT targetFramerateChanged();
        }
    }

    return m_mouseTrackingEnabled;
}

auto SettingsManager::setMouseTrackingEnabled(bool mouseTrackingEnabled) -> void
{
    if (m_mouseTrackingEnabled == mouseTrackingEnabled)
    {
        return;
    }

    if(!set(QString("mouseAllowed"), mouseTrackingEnabled ? QString("true") : QString("false")))
    {
        return;
    }

    m_mouseTrackingEnabled = mouseTrackingEnabled;
    emit mouseTrackingEnabledChanged();
}

auto SettingsManager::mouseTrackingBias() -> qreal
{
    QString value = get(QString("mouseSpeedBias"));

    if(value.isNull() || value.isEmpty())
    {
        setMouseTrackingBias(1.0);
    }
    else
    {
        bool okay = false;
        qreal convertedValue = value.toFloat(&okay);

        if(!okay)
        {
            setMouseTrackingBias(1.0);
        }

        if(convertedValue != m_mouseTrackingBias)
        {
            m_mouseTrackingBias = convertedValue;
            Q_EMIT mouseTrackingBiasChanged();
        }
    }

    return m_mouseTrackingBias;
}

auto SettingsManager::setMouseTrackingBias(qreal mouseTrackingBias) -> void
{
    if (qFuzzyCompare(m_mouseTrackingBias, mouseTrackingBias))
    {
        return;
    }

    if(!set(QString("mouseSpeedBias"), mouseTrackingBias))
    {
        return;
    }

    m_mouseTrackingBias = mouseTrackingBias;
    emit mouseTrackingBiasChanged();
}

auto SettingsManager::pauseMode() -> quint8
{
    QString value = get(QString("pauseMode"));

    if(value.isNull() || value.isEmpty())
    {
        setPauseMode(1);
    }
    else
    {
        bool okay = false;
        quint64 convertedValue = value.toULongLong(&okay);

        if(!okay)
        {
            setPauseMode(60);
        }

        if(convertedValue != m_pauseMode)
        {
            m_pauseMode = convertedValue;
            Q_EMIT pauseModeChanged();
        }
    }

    return m_pauseMode;
}

auto SettingsManager::setPauseMode(quint8 pauseMode) -> void
{
    if (m_pauseMode == pauseMode)
    {
        return;
    }

    if(!set(QString("pauseMode"), pauseMode))
    {
        return;
    }

    m_pauseMode = pauseMode;
    emit pauseModeChanged();
}

auto SettingsManager::resetPauseMode() -> void
{
    setPauseMode(1);
}

auto SettingsManager::onlyCheckActiveScreen() -> bool
{
    QString value = get(QString("checkActiveScreen"));

    if(value.isNull() || value.isEmpty())
    {
        setOnlyCheckActiveScreen(false);
    }
    else
    {
        bool convertedValue = value.trimmed().compare("true", Qt::CaseInsensitive);

        if(convertedValue != m_onlyCheckActiveScreen)
        {
            m_onlyCheckActiveScreen = convertedValue;
            Q_EMIT onlyCheckActiveScreenChanged();
        }
    }

    return m_onlyCheckActiveScreen;
}

auto SettingsManager::setOnlyCheckActiveScreen(bool onlyCheckActiveScreen) -> void
{
    if (m_onlyCheckActiveScreen == onlyCheckActiveScreen)
    {
        return;
    }

    if(!set(QString("checkActiveScreen"), onlyCheckActiveScreen ? QString("true") : QString("false")))
    {
        return;
    }

    m_onlyCheckActiveScreen = onlyCheckActiveScreen;
    emit onlyCheckActiveScreenChanged();
}

auto SettingsManager::resetOnlyCheckActiveScreen() -> void
{
    setOnlyCheckActiveScreen(false); // TODO: Adapt to use your actual default value
}

auto SettingsManager::excludedWindows() -> QStringList
{
    QString value = get(QString("excludeWindows"));

    QStringList convertedValue = value.split(QChar(','), Qt::SkipEmptyParts);

    if(convertedValue != m_excludedWindows)
    {
        m_excludedWindows = convertedValue;
        Q_EMIT excludedWindowsChanged();
    }

    return m_excludedWindows;
}

auto SettingsManager::setExcludedWindows(const QStringList &excludedWindows) -> void
{
    if (m_excludedWindows == excludedWindows)
    {
        return;
    }

    if(!set(QString("excludeWindows"), excludedWindows.join(QChar(','))))
    {
        return;
    }

    m_excludedWindows = excludedWindows;
    emit excludedWindowsChanged();
}

auto SettingsManager::resetExcludedWindows() -> void
{
    setExcludedWindows(QStringList{}); // TODO: Adapt to use your actual default value
}

auto SettingsManager::running() -> bool
{
    QString value = get(QString("running"));

    if(value.isNull() || value.isEmpty())
    {
        setRunning(true);
    }
    else
    {
        bool convertedValue = value.trimmed().compare("true", Qt::CaseInsensitive);

        if(convertedValue != m_running)
        {
            m_running = convertedValue;
            Q_EMIT runningChanged();
        }
    }

    return m_running;
}

auto SettingsManager::setRunning(bool running) -> void
{
    if (m_running == running)
    {
        return;
    }

    if(!set(QString("running"), running))
    {
        return;
    }

    m_running = running;
    emit runningChanged();
}

auto SettingsManager::resetRunning() -> void
{
    setRunning(true); // TODO: Adapt to use your actual default value
}

auto SettingsManager::jsonFromFile(const QString &uri) -> QJsonDocument
{
    QUrl url = QUrl(uri);
    url.setScheme(QString("file"));

    QFile file(url.toLocalFile());

    if(!file.exists())
    {
        return {};
    }

    if(!file.open(QFile::ReadOnly))
    {
        Q_EMIT error(QString("File Error"), file.errorString());

        return {};
    }

    QByteArray fileData = file.readAll();

    if(fileData.length() != file.size())
    {
        Q_EMIT error(QString("File Read Error"), file.errorString());
        
        return {};
    }

    QJsonParseError documentError; 
    QJsonDocument document = QJsonDocument::fromJson(fileData, &documentError);

    if(documentError.error != QJsonParseError::NoError)
    {
        switch (documentError.error) {

        case QJsonParseError::NoError:
        case QJsonParseError::UnterminatedObject:
        case QJsonParseError::MissingNameSeparator:
        case QJsonParseError::UnterminatedArray:
        case QJsonParseError::MissingValueSeparator:
        case QJsonParseError::IllegalValue:
        case QJsonParseError::TerminationByNumber:
        case QJsonParseError::IllegalNumber:
        case QJsonParseError::IllegalEscapeSequence:
        case QJsonParseError::IllegalUTF8String:
        case QJsonParseError::UnterminatedString:
        case QJsonParseError::MissingObject:
        case QJsonParseError::DeepNesting:
        case QJsonParseError::DocumentTooLarge:
        case QJsonParseError::GarbageAtEnd:
        //eventually we'll have a different message for each fail state
            Q_EMIT error(QString("Json Error"), documentError.errorString());
          break;
        }

        return {};
    }

    return document;
}

auto SettingsManager::packNameFromUri(const QString &uri) -> QString
{
    QJsonDocument document = jsonFromFile(uri);

    if(document.isNull())
    {
        Q_EMIT error(QString("File Error"), QString("File contains no json data"));

        return {};
    }

    QJsonObject rootObject = document.object();

    if(!rootObject.contains(QString("name")))
    {
        Q_EMIT error(QString("Json Error"), QString("Json is not a Komplex Pack"));

        return {};
    }

    return rootObject["name"].toString();
}

auto SettingsManager::uriFromPackName(const QString &name) -> QString
{
    Q_ASSERT_X(false,"SettingsManager","uriFromPackName not implemented");
}

auto SettingsManager::indexOfShaderPack(QString uri) -> qint64
{
    if(uri.isNull() || uri.isEmpty())
        uri = shaderPack();

    QUrl url = QUrl(uri);
    url.setScheme(QString("file"));

    // determine current shader pack index
    qint64 index = -1;

    for(int i = 0; i < m_availableShaderPacksMetadata.count(); ++i)
    {
        if(m_availableShaderPacksMetadata[i].uri == url.toLocalFile())
        {
            index = i;
            break;
        }
    }

    return index;
}