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
#ifndef VIDEOPAGINATOR_H
#define VIDEOPAGINATOR_H

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
#include "common/videocache.h"
#include "common/coreservices.h"

/**
 * @brief The VideoPaginator class
 * Most of the media endpoints for the Komplex API have the same data structure so
 * child classes can make use of this generic controller where possible
 */
class KOMPLEX_EXPORT VideoPaginator : public Paginator<VideoCache>
{
    Q_OBJECT
public:
    explicit VideoPaginator(QObject *parent = nullptr) : Paginator<VideoCache>(parent)
    {
        SlidingCacheController<VideoCache> *controller = this->controller();
        controller->setFetch(
            std::bind(&VideoPaginator::fetch, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    /**
     * @brief uri
     * URI of the API Endpoint. This is set by the child class
     * @return
     */
    [[nodiscard]]
    auto uri() const -> QString
    {
        return m_uri;
    }

    /**
     * @brief query
     * Current query string, if the API Endpoint supports queries
     * @return
     */
    [[nodiscard]]
    auto query() const -> QString
    {
        return m_query;
    }

    /**
     * @brief setQuery
     * Sets the new query string and updates the controller
     * @param query
     */
    auto setQuery(const QString &query) -> void
    {
        if(query == m_query)
        {
            return;
        }

        m_query = query;
        Q_EMIT queryChanged();

        reset();
        setOffset(0);
    }

signals:
    /**
     * @brief uriChanged
     */
    auto uriChanged() -> void;

    /**
     * @brief queryChanged
     */
    auto queryChanged() -> void;

protected:
    /**
     * @brief fetch
     * Used to fetch results from the API endpoint
     * for the CacheController. It is intended to be passed
     * to the Cache Controller not meant to be called directly.
     * @param offset Offset index to start the results from
     * @param limit Number of results to attempt to fetch
     * @return FetchResult<ImageCache>
     */
    [[nodiscard]]
    auto fetch(
        qsizetype offset,
        qsizetype limit
    ) const -> FetchResult<VideoCache>
    {
        LOG_ERROR_X(m_uri.isEmpty(),
            "VideoPaginator::fetch",
            "Uri not set",
            {}
        );

        QUrl url(
            QString::fromUtf8("%1/%2/%3").arg(
                KOMPLEX_API_HOST,
                KOMPLEX_API_VERSION,
                m_uri
            )
        );

        QNetworkRequest request(url);
        request.setHeaders(
            QHttpHeaders::fromMultiMap(
                {
                    {
                        QByteArray("query"),
                        m_query.toUtf8()
                    },
                    {
                        QByteArray("offset"),
                        QString::number(offset).toUtf8()
                    },
                    {
                        QByteArray("limit"),
                        QString::number(limit).toUtf8()
                    },
                    {
                        QByteArray("session_token"),
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
        const QJsonArray resultsArray = rootObject.value(QString::fromUtf8("videos")).toArray();

        QList<VideoCache> fetchedResults;

        for(const QJsonValue &result : resultsArray)
        {
            if(!result.isObject())
            {
                continue;
            }

            QJsonObject resultObject = result.toObject();

            QJsonObject userObject = resultObject.value(QString::fromUtf8("user")).toObject();
            QList<VideoEntry> videos;
            const QJsonArray videosArray = resultObject.value(QString::fromUtf8("video_files")).toArray();

            for(const QJsonValue &entry : videosArray)
            {
                if(!entry.isObject())
                {
                    continue;
                }

                QJsonObject videoObject = entry.toObject();

                videos.append
                (
                    {
                        videoObject.value(QString::fromUtf8("file_type")).toString(),
                        videoObject.value(QString::fromUtf8("fps")).toInteger(),
                        videoObject.value(QString::fromUtf8("height")).toInteger(),
                        videoObject.value(QString::fromUtf8("width")).toInteger(),
                        videoObject.value(QString::fromUtf8("size")).toInteger(),
                        videoObject.value(QString::fromUtf8("id")).toInteger(),
                        videoObject.value(QString::fromUtf8("link")).toString(),
                        videoObject.value(QString::fromUtf8("quality")).toString()
                    }
                );
            }

            fetchedResults.append
            (
                {
                    resultObject.value(QString::fromUtf8("avg_color")).toString(),
                    resultObject.value(QString::fromUtf8("duration")).toInteger(),
                    resultObject.value(QString::fromUtf8("height")).toInteger(),
                    resultObject.value(QString::fromUtf8("width")).toInteger(),
                    resultObject.value(QString::fromUtf8("id")).toInteger(),
                    resultObject.value(QString::fromUtf8("image")).toString(),
                    resultObject.value(QString::fromUtf8("url")).toString(),
                    userObject.value(QString::fromUtf8("name")).toString(),
                    userObject.value(QString::fromUtf8("url")).toString(),
                    userObject.value(QString::fromUtf8("id")).toInteger(),
                    std::move(videos)
                }
            );
        }

        FetchResult<VideoCache> result
        {
            static_cast<qsizetype>(rootObject.value(QString::fromUtf8("total_results")).toInteger()),
            fetchedResults.count(),
            std::move(fetchedResults)
        };

        return std::move(result);
    }

    /**
     * @brief getNetworkReply
     * Processes the request and blocks until completion.
     * @param request
     * @return
     */
    [[nodiscard]]
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

    /**
     * @brief getDocument
     * Extracts the JSON document from the server reply
     * @param reply
     * @return
     */
    [[nodiscard]]
    auto getDocument(QNetworkReply *reply) const -> QJsonDocument
    {
        QByteArray data = reply->readAll();
        QJsonParseError documentError;
        QJsonDocument document = QJsonDocument::fromJson(data, &documentError);

        LOG_ERROR_X(documentError.error != QJsonParseError::NoError,
            "NewestPacksPaginator::getDocument",
            documentError.errorString().toStdString().c_str(),
            {}
        );

        return document;
    }

    /**
     * @brief setUri
     * Sets the API Endpoint URI to fetch from
     * @param uri
     */
    auto setUri(const QString &uri) -> void
    {
        if(uri == m_uri)
        {
            return;
        }

        m_uri = uri;

        Q_EMIT uriChanged();
    }

private:
    /**
     * @brief m_uri
     * API Endpoint URI
     */
    QString m_uri;

    /**
     * @brief m_query
     * The query string used in fetch operations
     */
    QString m_query;
};

#endif // VIDEOPAGINATOR_H
