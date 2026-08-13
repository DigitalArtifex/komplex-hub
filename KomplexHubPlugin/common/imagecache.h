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
#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "komplex_global.h"
#include <QObject>
#include <QMap>
#include <QString>

struct KOMPLEX_EXPORT ImageCache
{
    QString altText;
    QByteArray averageColor;
    qsizetype height;
    qsizetype width;
    qint64 id;
    bool liked; //probably not needed
    QString photographer;
    QString photographerUrl;
    qint64 photographerId;
    QMap<QString, QString> sources;
    QMap<QString, QString> sourceSizes;
    QString url;

    auto operator == (const ImageCache &other) const -> bool
    {
        return (
            other.altText == altText &&
            other.averageColor == averageColor &&
            other.height == height &&
            other.width == width &&
            other.id == id &&
            other.liked == liked &&
            other.photographer == photographer &&
            other.photographerUrl == photographerUrl &&
            other.photographerId == photographerId &&
            other.sources == sources &&
            other.sourceSizes == sourceSizes &&
            other.url == url
        );
    }

    auto operator != (const ImageCache &other) const -> bool
    {
        return !(other == *this);
    }
};
Q_DECLARE_METATYPE(ImageCache)


#endif // IMAGECACHE_H
