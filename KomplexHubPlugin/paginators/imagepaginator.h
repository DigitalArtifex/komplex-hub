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
#ifndef IMAGEPAGINATOR_H
#define IMAGEPAGINATOR_H

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
#include "common/imagecache.h"
#include "common/coreservices.h"
#include "paginator.h"

/**
 * @brief The ImagePaginator class
 * Most of the media endpoints for the Komplex API have the same data structure so
 * child classes can make use of this generic controller where possible
 */
class KOMPLEX_EXPORT ImagePaginator : public Paginator<ImageCache>
{
    Q_OBJECT
public:
    explicit ImagePaginator(QObject *parent = nullptr) : Paginator<ImageCache>(parent)
    {
        SlidingCacheController<ImageCache> *controller = this->controller();
        controller->setFetch(
            std::bind(&ImagePaginator::fetch, this, std::placeholders::_1, std::placeholders::_2)
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

signals:
    /**
     * @brief uriChanged
     */
    auto uriChanged() -> void;

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
    ) const -> FetchResult<ImageCache>
    {
        LOG_ERROR_X(m_uri.isEmpty(),
            "ImagePaginator::fetch",
            "Uri not set",
            {}
        );

        if(queryable() && query().isEmpty())
        {
            return {};
        }

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
                        query().toUtf8()
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
        const QJsonArray resultsArray = rootObject.value(QString::fromUtf8("photos")).toArray();

        QList<ImageCache> fetchedResults;

        for(const auto result : resultsArray)
        {
            if(!result.isObject())
            {
                continue;
            }

            QJsonObject resultObject = result.toObject();

            const QVariantMap sources = resultObject.value(QString::fromUtf8("src")).toObject().toVariantMap();
            QMap<QString, QString> sourceMap;

            QMapIterator<QString,QVariant> sourceIterator(sources);
            QString original;

            while(sourceIterator.hasNext())
            {
                sourceIterator.next();

                if(sourceIterator.key().toLower() == QString::fromUtf8("original"))
                {
                    original = sourceIterator.value().toString();
                    break;
                }
            }

            QSize screenSize = CoreServices::screenSize();
            QSize portraitSize(screenSize.height(),screenSize.width());
            QSize landscapeSize(screenSize.width(),screenSize.height());

            if(screenSize.height() > screenSize.width())
            {
                portraitSize = screenSize;
                landscapeSize = QSize(screenSize.height(),screenSize.width());
            }

            sourceMap.clear();
            sourceMap = QMap<QString,QString>
            {
                {
                    QString::fromUtf8("original"),
                    original
                },
                {
                    QString::fromUtf8("screen"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=%2&w=%3").arg(
                        original,
                        QString::number(screenSize.height()),
                        QString::number(screenSize.width())
                    )
                },
                {
                    QString::fromUtf8("portrait"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=%2&w=%3").arg(
                        original,
                        QString::number(portraitSize.height()),
                        QString::number(portraitSize.width())
                    )
                },
                {
                    QString::fromUtf8("landscape"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=%2&w=%3").arg(
                        original,
                        QString::number(landscapeSize.height()),
                        QString::number(landscapeSize.width())
                    )
                },
                {
                    QString::fromUtf8("backgroundPortrait"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=%2&w=%3").arg(
                        original,
                        QString::number(portraitSize.height() / 2),
                        QString::number(portraitSize.width() / 2)
                    )
                },
                {
                    QString::fromUtf8("backgroundLandscape"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=%2&w=%3").arg(
                        original,
                        QString::number(landscapeSize.height() / 2),
                        QString::number(landscapeSize.width() / 2)
                    )
                },
                {
                    QString::fromUtf8("thumbnail"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=144&w=256%3").arg(
                        original
                    )
                },
                {
                    QString::fromUtf8("largeThumbnail"),
                    QString::fromUtf8("%1?auto=compress&cs=tinysrgb&fit=crop&h=450&w=800%3").arg(
                        original
                    )
                }
            };

            QMap<QString, QString> sizeMap =
            {
                {
                    QString::fromUtf8("original"),
                    QString()
                },
                {
                    QString::fromUtf8("screen"),
                    QString::fromUtf8("%1x%2").arg(
                        QString::number(screenSize.width()),
                        QString::number(screenSize.height())
                    )
                },
                {
                    QString::fromUtf8("portrait"),
                    QString::fromUtf8("%1x%2").arg(
                        QString::number(portraitSize.width()),
                        QString::number(portraitSize.height())
                    )
                },
                {
                    QString::fromUtf8("landscape"),
                    QString::fromUtf8("%1x%2").arg(
                        QString::number(landscapeSize.width()),
                        QString::number(landscapeSize.height())
                    )
                }
            };

            fetchedResults.append(
                {
                    resultObject.value(QString::fromUtf8("alt")).toString(),
                    resultObject.value(QString::fromUtf8("avg_color")).toString().toUtf8(),
                    resultObject.value(QString::fromUtf8("height")).toInteger(),
                    resultObject.value(QString::fromUtf8("width")).toInteger(),
                    resultObject.value(QString::fromUtf8("id")).toInteger(),
                    resultObject.value(QString::fromUtf8("liked")).toBool(),
                    resultObject.value(QString::fromUtf8("photographer")).toString(),
                    resultObject.value(QString::fromUtf8("photographer_url")).toString(),
                    resultObject.value(QString::fromUtf8("photographer_id")).toInteger(),
                    std::move(sourceMap),
                    std::move(sizeMap),
                    resultObject.value(QString::fromUtf8("url")).toString()
                }
            );
        }

        FetchResult<ImageCache> result
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

        manager.clear();

        LOG_ERROR_X(reply->error() != QNetworkReply::NoError,
            "NewestPacksPaginator::getNetworkReply",
            reply->errorString().toStdString().c_str(),
            nullptr
        );

        return reply;
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
     * @brief s_sizeExpression
     * Regular expression used to extract size data from the url
     */
    inline static QRegularExpression s_sizeExpression = QRegularExpression
    (
        QString::fromUtf8("[h]\\=\\d{1,}\\&[w]\\=\\d{1,}")
    );
};

#endif // IMAGEPAGINATOR_H
