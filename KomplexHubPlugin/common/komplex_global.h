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
#ifndef KOMPLEX_GLOBAL_H
#define KOMPLEX_GLOBAL_H
#include <QtTypes>
#include <QString>
#include <chrono>

#ifdef KOMPLEX_PLUGIN
#define KOMPLEX_EXPORT Q_DECL_EXPORT
#else
#define KOMPLEX_EXPORT Q_DECL_IMPORT
#endif

#define SLIDING_CACHE_WINDOW_SIZE 100
#define SLIDING_CACHE_PREFETCH_THRESHOLD 20

#define NETWORK_TIMEOUT 10000
#define NETWORK_RETRY_LIMIT 3

#ifdef KOMPLEX_LOCAL_DEV
#define KOMPLEX_API_HOST                        "https://komplexapi.hlab"
#else
inline const QString KOMPLEX_API_HOST                         = QStringLiteral("https://api.komplex.dev");
#endif

inline const QString KOMPLEX_API_VERSION                      = QStringLiteral("v2");

inline const QString KOMPLEX_ENDPOINT_IMAGES_SEARCH           = QStringLiteral("images/search");
inline const QString KOMPLEX_ENDPOINT_IMAGES_FEATURED         = QStringLiteral("featured/images");
inline const QString KOMPLEX_ENDPOINT_IMAGES_ITEM             = QStringLiteral("images/item");

inline const QString KOMPLEX_ENDPOINT_VIDEOS_SEARCH           = QStringLiteral("videos/search");
inline const QString KOMPLEX_ENDPOINT_VIDEOS_FEATURED         = QStringLiteral("featured/videos");
inline const QString KOMPLEX_ENDPOINT_VIDEOS_ITEM             = QStringLiteral("videos/item");

inline const QString KOMPLEX_ENDPOINT_SHADERS_SEARCH          = QStringLiteral("shaders/search");
inline const QString KOMPLEX_ENDPOINT_SHADERS_FEATURED        = QStringLiteral("shaders/featured");
inline const QString KOMPLEX_ENDPOINT_SHADERS_ITEM            = QStringLiteral("shaders/item");
inline const QString KOMPLEX_ENDPOINT_SHADERS_NEWEST          = QStringLiteral("shaders/newest");

inline const QString KOMPLEX_ENDPOINT_CUBEMAPS_SEARCH         = QStringLiteral("cubemaps/search");
inline const QString KOMPLEX_ENDPOINT_CUBEMAPS_FEATURED       = QStringLiteral("cubemaps/featured");
inline const QString KOMPLEX_ENDPOINT_CUBEMAPS_ITEM           = QStringLiteral("cubemaps/item");
inline const QString KOMPLEX_ENDPOINT_CUBEMAPS_NEWEST         = QStringLiteral("cubemaps/newest");

inline const QString KOMPLEX_ENDPOINT_PACKS_SEARCH            = QStringLiteral("packs/search");
inline const QString KOMPLEX_ENDPOINT_PACKS_FEATURED          = QStringLiteral("packs/featured");
inline const QString KOMPLEX_ENDPOINT_PACKS_ITEM              = QStringLiteral("packs/item");
inline const QString KOMPLEX_ENDPOINT_PACKS_NEWEST            = QStringLiteral("packs/newest");

inline const QString KOMPLEX_ENDPOINT_USER_AUTH               = QStringLiteral("user/auth");

inline const qsizetype nullsize                               = std::numeric_limits<qsizetype>::min();
inline const std::chrono::milliseconds KOMPLEX_RATE_LIMIT(500);
inline const QString KOMPLEX_KEYCHAIN_SERVICE_NAME = QStringLiteral
(
    "com.digitalartifex.komplex"
);

#endif // KOMPLEX_GLOBAL_H
