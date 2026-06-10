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
#ifndef WALLPAPERCACHE_H
#define WALLPAPERCACHE_H

#include "komplex_global.h"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDateTime>

struct KOMPLEX_EXPORT WallpaperInstallData
{
    QString uri;
    QString name;
    QString author;
    QString description;
    QString thumbnail;
    QDateTime installDate;
};
Q_DECLARE_METATYPE(WallpaperInstallData)

struct KOMPLEX_EXPORT WallpaperCache
{
    QString uuid;
    QString name;
    QString author;
    QString authorId;
    QString description;
    QString thumbnail;
    QDateTime createdDate;
    qreal price = 0;
    QString currency;
    quint64 downloadCount = 0;
    qint8 type = -1;

    auto operator == (const WallpaperCache &other) const -> bool
    {
        return (
            other.uuid == uuid &&
            other.name == name &&
            other.author == author &&
            other.authorId == authorId &&
            other.description == description &&
            other.thumbnail == thumbnail &&
            other.createdDate == createdDate &&
            other.price == price &&
            other.currency == currency &&
            other.downloadCount == downloadCount &&
            other.type == type
        );
    }

    auto operator != (const WallpaperCache &other) const -> bool
    {
        return !(other == *this);
    }
};
Q_DECLARE_METATYPE(WallpaperCache)

#endif // WALLPAPERCACHE_H
