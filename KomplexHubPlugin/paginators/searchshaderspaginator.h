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
#ifndef SEARCHSHADERSPAGINATOR_H
#define SEARCHSHADERSPAGINATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include "wallpaperpaginator.h"

class KOMPLEX_EXPORT SearchShadersPaginator : public WallpaperPaginator
{
public:
    explicit SearchShadersPaginator() : WallpaperPaginator()
    {
        setUri(
            QString::fromUtf8(KOMPLEX_ENDPOINT_SHADERS_SEARCH)
        );
    }
};

#endif // SEARCHSHADERSPAGINATOR_H
