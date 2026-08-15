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
#ifndef FEATUREDIMAGESPAGINATOR_H
#define FEATUREDIMAGESPAGINATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include "imagepaginator.h"

class KOMPLEX_EXPORT FeaturedImagesPaginator : public ImagePaginator
{
public:
    explicit FeaturedImagesPaginator(QObject *parent = nullptr) : ImagePaginator(parent)
    {
        setUri(KOMPLEX_ENDPOINT_IMAGES_FEATURED);
        controller()->setWindowSize(32);
    }
};

#endif // FEATUREDIMAGESPAGINATOR_H
