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
    auto dataChanged() -> void;
    auto resultsPerPageChanged() -> void;
    auto totalResultsChanged() -> void;
    auto totalPagesChanged() -> void;
    auto pageChanged() -> void;
    auto countChanged() -> void;
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

    }

    /**
     * @brief next
     *
     * Attempts to get the next page and notifies pageChanged
     */
    auto next() -> void
    {
        setOffset(m_offset + m_resultsPerPage);
    }

    /**
     * @brief previous
     *
     * Attempts to get the previous page and notifies pageChanged
     */
    auto previous() -> void
    {
        setOffset(m_offset - m_resultsPerPage);
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
        QMutexLocker dataLocker(&m_dataMutex);

        if(resultsPerPage == m_resultsPerPage)
        {
            return;
        }

        m_resultsPerPage = resultsPerPage;
        Q_EMIT static_cast<PaginationNotifier*>(this)->resultsPerPageChanged();

        m_data = m_cacheController.chunk(m_offset, m_resultsPerPage);
        Q_EMIT static_cast<PaginationNotifier*>(this)->dataChanged();
    }

    /**
     * @brief data
     * @return
     */
    auto data() const -> QList<T>
    {
        return m_data;
    }

    /**
     * @brief count
     * @return
     */
    auto count() const -> qsizetype
    {
        return m_data.count();
    }

    /**
     * @brief at
     * @param index
     * @return
     */
    auto at(qsizetype index) const -> T
    {
        QMutexLocker dataLocker(&m_dataMutex);

        if(index < 0 || index >= m_data.count())
        {
            return {};
        }

        return m_data.at(index);
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
        if(offset == m_offset && offset != std::numeric_limits<qsizetype>::infinity())
        {
            return;
        }

        if(offset == std::numeric_limits<qsizetype>::infinity())
        {
            offset = 0;
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

        QList<T> data = m_cacheController.chunk(m_offset, m_resultsPerPage);
        setData(data);

        Q_EMIT this->pageChanged();
    }

    /**
     * @brief setData
     * @param data
     */
    auto setData(const QList<T> &data)
    {
        m_dataMutex.lock();

        m_data.clear();
        m_data.append(std::move(data));
        m_dataMutex.unlock();

        Q_EMIT this->countChanged();
        Q_EMIT this->dataChanged();
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
        m_dataMutex.lock();

        m_data.clear();
        controller()->reset();
        m_dataMutex.unlock();

        Q_EMIT totalPagesChanged();
        Q_EMIT dataChanged();
        Q_EMIT pageChanged();

        setOffset(std::numeric_limits<qsizetype>::infinity());
    }

private:

    /**
     * @brief m_offset
     * Current absolute offset. Used in fetch operations
     */
    qsizetype m_offset = std::numeric_limits<qsizetype>::infinity();

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
     * @brief m_data
     * Internal data copy
     */
    QList<T> m_data;

    /**
     * @brief m_dataMutex
     */
    mutable QMutex m_dataMutex;
};

#endif // PAGINATOR_H
