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
#ifndef NEWESTPACKSPAGINATOR_H
#define NEWESTPACKSPAGINATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include "common/logging.h"
#include "common/paginator.h"
#include "common/wallpapercache.h"
#include "common/coreservices.h"

class KOMPLEX_EXPORT NewestPacksPaginator : public Paginator<WallpaperCache>
{

public:
    explicit NewestPacksPaginator() : Paginator<WallpaperCache>()
    {
        SlidingCacheController<WallpaperCache> *controller = this->controller();
        controller->setFetch(
            std::bind(&NewestPacksPaginator::fetch, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

protected:
    /**
     * @brief fetch
     * Used to fetch results from the API endpoint
     * for the CacheController. It is intended to be passed
     * to the Cache Controller not meant to be called directly.
     * @param offset Offset index to start the results from
     * @param limit Number of results to attempt to fetch
     * @return FetchResult<WallpaperCache>
     */
    auto fetch(
        qsizetype offset,
        qsizetype limit
    ) const -> FetchResult<WallpaperCache>
    {
        QUrl url(
            QString::fromUtf8("%1/v2/packs/newest/").arg(KOMPLEX_API_HOST)
        );

        QNetworkRequest request(url);
        request.setHeaders(
            QHttpHeaders::fromMultiMap(
                {
                    {
                        QByteArray("Offset"),
                        QString::number(offset).toUtf8()
                    },
                    {
                        QByteArray("Limit"),
                        QString::number(limit).toUtf8()
                    },
                    {
                        QByteArray("SessionToken"),
                        CoreServices::sessionToken()
                    }
                }
            )
        );

        QNetworkReply *reply = getNetworkReply(request);

        if(reply == nullptr)
        {
            return {};
        }

        QJsonDocument document = getDocument(reply);

        QJsonObject rootObject = document.object();
        const QJsonArray resultsArray = rootObject.value(QString::fromUtf8("results")).toArray();

        QList<WallpaperCache> fetchedResults;

        for(const auto result : resultsArray)
        {
            if(!result.isObject())
            {
                continue;
            }

            QJsonObject resultObject = result.toObject();

            fetchedResults.append(
                {
                    resultObject.value(QString::fromUtf8("uri")).toString(),
                    resultObject.value(QString::fromUtf8("name")).toString(),
                    resultObject.value(QString::fromUtf8("author")).toString(),
                    resultObject.value(QString::fromUtf8("authorId")).toString(),
                    resultObject.value(QString::fromUtf8("description")).toString(),
                    resultObject.value(QString::fromUtf8("thumbnail")).toString(),
                    QDateTime::fromString(
                        resultObject.value(QString::fromUtf8("createdDate")).toString()
                    ),
                    resultObject.value(QString::fromUtf8("price")).toDouble(),
                    resultObject.value(QString::fromUtf8("currency")).toString(),
                    static_cast<quint64>(
                        resultObject.value(QString::fromUtf8("downloadCount")
                    ).toInteger()),
                    static_cast<qint8>(
                        resultObject.value(QString::fromUtf8("description")
                    ).toInt()),
                }
            );
        }

        return
        {
            static_cast<qsizetype>(rootObject.value(QString::fromUtf8("total_results")).toInteger()),
            fetchedResults.count(),
            std::move(fetchedResults)
        };
    }

    /**
     * @brief getNetworkReply
     * Processes the request and blocks until completion.
     * @param request
     * @return
     */
    auto getNetworkReply(const QNetworkRequest &request) const -> QNetworkReply*
    {
        QEventLoop loop;
        QWeakPointer<QNetworkAccessManager> managerReference = CoreServices::networkAccessManager();

        QSharedPointer<QNetworkAccessManager> manager = managerReference.toStrongRef();

        LOG_ERROR_X(!manager,
            "NewestPacksPaginator::getNetworkReply",
            "Network manager reference has already been deleted",
            nullptr
        );

        QNetworkReply *reply = manager->post(request, nullptr);
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        QObject::connect(reply, &QNetworkReply::errorOccurred, &loop,
            [&loop](QNetworkReply::NetworkError error) -> void
            {
                loop.quit();
            }
        );

        if(!reply->isFinished())
        {
            loop.exec();
        }

        LOG_ERROR_X(reply->error() != QNetworkReply::NoError,
            "NewestPacksPaginator::getNetworkReply",
            reply->errorString().toStdString().c_str(),
            nullptr
        );

        return reply;
    }

    auto getDocument(QNetworkReply *reply) const -> QJsonDocument
    {
        QJsonParseError documentError;
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll(), &documentError);

        LOG_ERROR_X(documentError.error != QJsonParseError::NoError,
            "NewestPacksPaginator::getDocument",
            documentError.errorString().toStdString().c_str(),
            {}
        );

        return document;
    }
};

#endif // NEWESTPACKSPAGINATOR_H
