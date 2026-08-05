/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex
 *  https://digitalartifex.dev - https://github.com/DigitalArtifex
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
#ifndef CORESERVICES_H
#define CORESERVICES_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QScreen>
#include <QMutex>
#include <QMutexLocker>

class CoreServices
{
public:

    /**
     * @brief networkAccessManager
     * Plugin-wide QNetworkAccessManager
     * @return
     */
    static auto networkAccessManager() -> QWeakPointer<QNetworkAccessManager>;

    /**
     * @brief getSessionToken
     * Plugin-wide access to current user session token
     * @return
     */
    static auto sessionToken() -> QByteArray;

    static auto screenSize() -> QSize;

private:

    CoreServices() = default;
    ~CoreServices() = default;
    CoreServices(const CoreServices&) = delete;
    CoreServices& operator=(const CoreServices&) = delete;

    static QMutex s_networkAccessMutex;
    static QNetworkAccessManager *s_networkAccessManager;
    static QSharedPointer<QNetworkAccessManager> s_networkAccessPointer;
};
#endif // CORESERVICES_H
