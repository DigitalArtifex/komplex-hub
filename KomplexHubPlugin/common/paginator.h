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
#ifndef PAGINATOR_H
#define PAGINATOR_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <qdebug.h>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>

#include "komplex_global.h"
#include "slidingcachecontroller.h"

/**
 * @brief The PaginationNotifier class
 *
 * Qt does not support templated QObjects. Pushing it to a notifier class
 * that the template can inherit fixes this
 */
class KOMPLEX_EXPORT PaginationNotifier : public QObject
{
    Q_OBJECT
protected:
    explicit PaginationNotifier(QObject *parent = nullptr) : QObject(parent) {}
signals:
    auto fetchComplete() -> void;
    auto fetching() -> void;
    auto resultsPerPageChanged() -> void;
    auto totalResultsChanged() -> void;
    auto totalPagesChanged() -> void;
    auto pageChanged() -> void;
    auto countChanged() -> void;
    auto queryChanged() -> void;
    auto uriChanged() -> void;
};

/**
 * @brief The Paginator class is a type agnostic pagination controller that uses
 * the SlidingCacheController as it's backend
 */
template<typename T>
class KOMPLEX_EXPORT Paginator : public PaginationNotifier
{
public:
    explicit Paginator(QObject *parent = nullptr) : PaginationNotifier(parent)
    {
        QObject::connect
        (
            &m_cacheController,
            &SlidingCacheNotifier::fetchComplete,
            this,
            &PaginationNotifier::fetchComplete
        );

        QObject::connect
        (
            &m_cacheController,
            &SlidingCacheNotifier::fetching,
            this,
            &PaginationNotifier::fetching
        );
    }

    /**
     * @brief next
     *
     * Attempts to get the next page and notifies pageChanged
     */
    auto next() -> void
    {
        if(m_offset != nullsize)
        {
            setOffset(m_offset + m_resultsPerPage);
        }
        else
        {
            controller()->init();
            setOffset(0);
        }
    }

    /**
     * @brief previous
     *
     * Attempts to get the previous page and notifies pageChanged
     */
    auto previous() -> void
    {
        if(m_offset != nullsize && m_offset > m_resultsPerPage)
        {
            setOffset(m_offset - m_resultsPerPage);
        }
        else
        {
            controller()->init();
            setOffset(0);
        }
    }

    /**
     * @brief resultsPerPage
     * @return
     */
    auto resultsPerPage() const -> qsizetype
    {
        return m_resultsPerPage;
    }

    /**
     * @brief setResultsPerPage
     * @param resultsPerPage
     */
    auto setResultsPerPage(qsizetype resultsPerPage) -> void
    {
        if(resultsPerPage == m_resultsPerPage)
        {
            return;
        }

        m_resultsPerPage = resultsPerPage;
        Q_EMIT static_cast<PaginationNotifier*>(this)->resultsPerPageChanged();
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

        reset();

        m_query = query;
        Q_EMIT queryChanged();

        controller()->init();
        setOffset(0);
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

    /**
     * @brief data
     * @return
     */
    auto data() const -> const QList<T>
    {
        if(boundaryCheck(m_resultsPerPage))
        {
            return m_cacheController.chunk(m_offset, m_resultsPerPage);
        }

        qsizetype count = (m_cacheController.totalCount() - m_offset);
        return m_cacheController.chunk(m_offset, count);
    }

    /**
     * @brief count
     * @return
     */
    auto count() const -> qsizetype
    {
        if(m_offset == nullsize)
        {
            return 0;
        }

        if(boundaryCheck(m_resultsPerPage))
        {
            return m_resultsPerPage;
        }

        qsizetype count = (m_cacheController.totalCount() - m_offset);
        return count;
    }

    /**
     * @brief at
     * @param index
     * @return
     */
    auto at(qsizetype index) const -> const T&
    {
        return m_cacheController.at(m_offset + index);
    }

    /**
     * @brief totalResults
     *
     * The total number of results as reported by the controller
     * @return
     */
    auto totalResults() const -> qsizetype
    {
        return m_cacheController.totalCount();
    }

    /**
     * @brief page
     * Calculates the current page
     * @return
     */
    auto page() const -> qsizetype
    {
        return std::floor(
            static_cast<qreal>(m_offset) / m_resultsPerPage
        );
    }

    /**
     * @brief totalPages
     * Calculates the total number of pages available
     * @return
     */
    auto totalPages() const -> qsizetype
    {
        return std::ceil(
            static_cast<qreal>(totalResults()) / m_resultsPerPage
        );
    }

protected:

    auto queryable() const -> bool
    {
        return m_queryable;
    }

    auto setQueryable(bool queryable) -> void
    {
        m_queryable = queryable;
    }

    /**
     * @brief controller
     * Reference to the underlying cache controller
     * @return
     */
    auto controller() -> SlidingCacheController<T>*
    {
        return &m_cacheController;
    }

    /**
     * @brief setOffset
     * Sets the dataset to the specified offset
     * @param offset
     */
    auto setOffset(qsizetype offset) -> void
    {
        if(offset == m_offset)
        {
            return;
        }

        m_offset = offset;

        if(m_offset >= totalResults())
        {
            m_offset = totalResults() - m_resultsPerPage;
        }

        if(m_offset < 0)
        {
            m_offset = 0;
        }

        m_cacheController.focus(offset, m_resultsPerPage);

        Q_EMIT totalResultsChanged();
        Q_EMIT totalPagesChanged();
        Q_EMIT pageChanged();
    }

    /**
     * @brief onControllerTotalResultsChanged
     * Slot used to forward totalResultsChanged signal
     */
    auto onControllerTotalResultsChanged() -> void
    {
        Q_EMIT this->totalResultsChanged();
    }

    /**
     * @brief reset
     * Resets the paginator and underlying controller
     */
    auto reset() -> void
    {
        m_query.clear();
        m_offset = nullsize;
        controller()->reset();
    }

    /**
     * @brief getDocument
     * Extracts the JSON document from the server reply and deletes the reply
     * object.
     * @param reply
     * @return
     */
    [[nodiscard]]
    auto getDocument(QNetworkReply *reply) const -> QJsonDocument
    {
        QByteArray data = reply->readAll();
        QJsonParseError documentError;
        QJsonDocument document = QJsonDocument::fromJson(data, &documentError);

        reply->deleteLater();

        LOG_ERROR_X(documentError.error != QJsonParseError::NoError,
            "NewestPacksPaginator::getDocument",
            documentError.errorString().toStdString().c_str(),
            {}
        );

        return document;
    }

private:

    /**
     * @brief boundaryCheck
     * Checks if the index is within the bounds of the dataset
     * @param index
     * @return
     */
    auto boundaryCheck(qsizetype index) const -> bool
    {
        if(index < 0 || (m_offset + index) >= m_cacheController.totalCount())
        {
            return false;
        }

        return true;
    }

    /**
     * @brief m_offset
     * Current absolute offset. Used in fetch operations
     */
    mutable qsizetype m_offset = nullsize;

    /**
     * @brief m_resultsPerPage
     * Results per page to use when calculating current and total pages.
     * Needs to be set by the view
     */
    qsizetype m_resultsPerPage = 0;

    /**
     * @brief m_cacheController
     * Internal cache controller.
     *
     * TODO: make the cache controller generic so it can be used in other projects
     * and with other cache methods
     */
    SlidingCacheController<T> m_cacheController;

    /**
     * @brief m_dataMutex
     */
    mutable QMutex m_dataMutex;

    /**
     * @brief m_query
     * The query string used in fetch operations
     */
    QString m_query;

    /**
     * @brief m_queryable
     * Controls the queryable state of the paginator
     */
    bool m_queryable = false;

    /**
     * @brief m_uri
     * API Endpoint URI
     */
    QString m_uri;
};

#endif // PAGINATOR_H
