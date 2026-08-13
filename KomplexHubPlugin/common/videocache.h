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
#ifndef VIDEOCACHE_H
#define VIDEOCACHE_H

#include "komplex_global.h"

#include <QObject>

struct KOMPLEX_EXPORT VideoEntry
{
    QString type;
    qint64 fps = std::numeric_limits<qint64>::infinity();
    qint64 height = std::numeric_limits<qint64>::infinity();
    qint64 width = std::numeric_limits<qint64>::infinity();
    qint64 size = std::numeric_limits<qint64>::infinity();
    qint64 id = std::numeric_limits<qint64>::infinity();
    QString url;
    QString quality;

    auto operator == (const VideoEntry &other) const -> bool
    {
        return (
            other.fps == fps &&
            other.height == height &&
            other.id == id &&
            other.quality == quality &&
            other.size == size &&
            other.type == type &&
            other.url == url &&
            other.width == width
        );
    }

    auto operator != (const VideoEntry &other) const -> bool
    {
        return !(other == *this);
    }
};
Q_DECLARE_METATYPE(VideoEntry)

struct KOMPLEX_EXPORT VideoCache
{
    QString averageColor;
    qint64 duration = std::numeric_limits<qint64>::infinity();
    qint64 height = std::numeric_limits<qint64>::infinity();
    qint64 width = std::numeric_limits<qint64>::infinity();
    qint64 id = std::numeric_limits<qint64>::infinity();
    QString thumbnail;
    QString url;
    QString author;
    QString authorUrl;
    qint64 authorId;
    QList<VideoEntry> files;

    auto operator == (const VideoCache &other) const -> bool
    {
        return (
            other.averageColor == averageColor &&
            other.duration == duration &&
            other.height == height &&
            other.width == width &&
            other.id == id &&
            other.thumbnail == thumbnail &&
            other.url == url &&
            other.author == author &&
            other.authorUrl == authorUrl &&
            other.authorId == authorId &&
            other.files == files
        );
    }

    auto operator != (const VideoCache &other) const -> bool
    {
        return !(other == *this);
    }
};

#endif // VIDEOCACHE_H
