/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex | github.com/DigitalArtifex
 *
 *  SettingsManager.h
 * 
 *  This class provides access to Komplex Wallpaper settings via kwrite/
 *  kreadconfig 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>
 */

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H
#include "Komplex_global.h"

#include <QVariant>
#include <QObject>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QDir>
#include <QMap>
#include <QProcess>
#include <QEventLoop>
#include <QtQml/qqmlregistration.h>
#include <qjsondocument.h>
#include <qobject.h>

class SettingsManager;

struct KOMPLEX_EXPORT WallpaperLocationData
{
    QString name;
    QString uri;
};

Q_DECLARE_METATYPE(WallpaperLocationData)

class KOMPLEX_EXPORT SettingsManager : public QObject 
{
    Q_OBJECT
    QML_ELEMENT

public:

    explicit SettingsManager(QObject *parent = nullptr);
    ~SettingsManager();

    /**!
     * @brief get
     * This function gets the current setting for the supplied key.
     * The value will need to be converted from a string to the type
     * needed in code
     * 
     * @param key The key of the setting to retrieve
     * @returns The value of the supplied key, as a string
     */
    [[nodiscard]]
    Q_INVOKABLE auto get(const QString &key) -> QString;

    /**!
     * @brief set
     * This function sets the given value for the supplied key
     * 
     * @param key The key of the setting
     * @param value The value to set for the key
     */
    [[nodiscard]]
    Q_INVOKABLE auto set(const QString &key, const QVariant &value) -> bool;

    [[nodiscard]]
    auto shaderPack() -> QString;

    auto setShaderPack(const QString &uri) -> void;

    [[nodiscard]]
    auto shaderPackName() const -> QString;

    [[nodiscard]]
    auto availableShaderPacks() -> QStringList;

    [[nodiscard]]
    auto homeLocation() const -> QString;

    [[nodiscard]]
    auto configLocation() const -> QString;

    [[nodiscard]]
    auto shaderPackLocation() const -> QString;

    [[nodiscard]]
    auto shaderPackIndex() const -> qint64;
    auto setShaderPackIndex(qint64 shaderPackIndex) -> void;

    auto setShaderPackName(const QString &shaderPackName) -> void;

    [[nodiscard]]
    auto resolutionX() -> quint64;
    auto setResolutionX(quint64 resolutionX) -> void;

    [[nodiscard]]
    auto resolutionY() -> quint64;
    auto setResolutionY(quint64 resolutionY) -> void;

    [[nodiscard]]
    auto targetFramerate() -> quint64;
    auto setTargetFramerate(quint64 targetFramerate) -> void;

    [[nodiscard]]
    auto shaderSpeed() -> qreal;
    auto setShaderSpeed(qreal shaderSpeed) -> void;

    [[nodiscard]]
    auto mouseTrackingEnabled() -> bool;
    auto setMouseTrackingEnabled(bool mouseTrackingEnabled) -> void;

    [[nodiscard]]
    auto mouseTrackingBias() -> qreal;
    auto setMouseTrackingBias(qreal mouseTrackingBias) -> void;

    [[nodiscard]]
    auto pauseMode() -> quint8;
    auto setPauseMode(quint8 pauseMode) -> void;
    auto resetPauseMode() -> void;

    [[nodiscard]]
    auto onlyCheckActiveScreen() -> bool;
    auto setOnlyCheckActiveScreen(bool onlyCheckActiveScreen) -> void;
    auto resetOnlyCheckActiveScreen() -> void;

    [[nodiscard]]
    auto excludedWindows() -> QStringList;
    auto setExcludedWindows(const QStringList &excludedWindows) -> void;
    auto resetExcludedWindows() -> void;

    [[nodiscard]]
    auto running() -> bool;
    auto setRunning(bool running) -> void;
    auto resetRunning() -> void;

signals:
    auto shaderPackChanged() -> void;
    auto shaderPackNameChanged() -> void;
    auto availableShaderPacksChanged() -> void;
    auto error(const QString&, const QString&) -> void;
    auto shaderPackIndexChanged() -> void;
    auto resolutionXChanged() -> void;
    auto resolutionYChanged() -> void;
    auto targetFramerateChanged() -> void;
    auto shaderSpeedChanged() -> void;
    auto mouseTrackingEnabledChanged() -> void;
    auto mouseTrackingBiasChanged() -> void;
    auto pauseModeChanged() -> void;
    auto onlyCheckActiveScreenChanged() -> void;
    auto excludedWindowsChanged() -> void;
    auto runningChanged() -> void;

private:
    QString m_homeLocation;
    QString m_configLocation;
    QString m_shaderPackLocation;

    QString m_shaderPack = QString();
    QString m_shaderPackName = QString();
    qint64  m_shaderPackIndex = -1;
    qreal   m_shaderSpeed = 1.0;
    QStringList m_availableShaderPacks = QStringList();
    QList<WallpaperLocationData> m_availableShaderPacksMetadata;

    quint64 m_resolutionX = 0;
    quint64 m_resolutionY = 0;

    quint64 m_targetFramerate = 60;

    bool m_mouseTrackingEnabled = false;
    qreal m_mouseTrackingBias = 1.0;

    quint8 m_pauseMode = 1;

    bool m_onlyCheckActiveScreen = false;

    QStringList m_excludedWindows;

    bool m_running = true;
    
    QJsonDocument jsonFromFile(const QString &uri);

    QString packNameFromUri(const QString &uri);
    QString uriFromPackName(const QString &name);

    qint64 indexOfShaderPack(QString uri = QString());

    Q_PROPERTY(QString shaderPack READ shaderPack WRITE setShaderPack NOTIFY shaderPackChanged);
    Q_PROPERTY(QString shaderPackName READ shaderPackName NOTIFY shaderPackNameChanged);
    Q_PROPERTY(QStringList availableShaderPacks READ availableShaderPacks NOTIFY availableShaderPacksChanged);
    Q_PROPERTY(qint64 shaderPackIndex READ shaderPackIndex WRITE setShaderPackIndex NOTIFY shaderPackIndexChanged FINAL)
    Q_PROPERTY(quint64 resolutionX READ resolutionX WRITE setResolutionX NOTIFY resolutionXChanged FINAL)
    Q_PROPERTY(quint64 resolutionY READ resolutionY WRITE setResolutionY NOTIFY resolutionYChanged FINAL)
    Q_PROPERTY(quint64 targetFramerate READ targetFramerate WRITE setTargetFramerate NOTIFY targetFramerateChanged FINAL)
    Q_PROPERTY(qreal shaderSpeed READ shaderSpeed WRITE setShaderSpeed NOTIFY shaderSpeedChanged FINAL)
    Q_PROPERTY(bool mouseTrackingEnabled READ mouseTrackingEnabled WRITE setMouseTrackingEnabled NOTIFY mouseTrackingEnabledChanged FINAL)
    Q_PROPERTY(qreal mouseTrackingBias READ mouseTrackingBias WRITE setMouseTrackingBias NOTIFY mouseTrackingBiasChanged FINAL)
    Q_PROPERTY(quint8 pauseMode READ pauseMode WRITE setPauseMode RESET resetPauseMode NOTIFY pauseModeChanged FINAL)
    Q_PROPERTY(bool onlyCheckActiveScreen READ onlyCheckActiveScreen WRITE setOnlyCheckActiveScreen RESET resetOnlyCheckActiveScreen NOTIFY onlyCheckActiveScreenChanged FINAL)
    Q_PROPERTY(QStringList excludedWindows READ excludedWindows WRITE setExcludedWindows RESET resetExcludedWindows NOTIFY excludedWindowsChanged FINAL)
    Q_PROPERTY(bool running READ running WRITE setRunning RESET resetRunning NOTIFY runningChanged FINAL)
};

Q_DECLARE_METATYPE(SettingsManager)
#endif