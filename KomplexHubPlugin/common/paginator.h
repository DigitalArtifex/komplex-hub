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

#include "komplex_global.h"
#include "slidingcachecontroller.h"

/**
 * @brief The PaginatorNotifier class
 *
 * Qt does not support templated QObjects. Pushing it to a notifier class
 * that the template can inherit fixes this
 */
class KOMPLEX_EXPORT PaginatorNotifier : public QObject
{
    Q_OBJECT
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
class KOMPLEX_EXPORT Paginator : public PaginatorNotifier
{
public:
    /**
     * @brief next
     *
     * Attempts to get the next page and notifies pageChanged
     */
    auto next() const -> void
    {
        setOffset(m_offset + m_resultsPerPage);
        Q_EMIT static_cast<PaginatorNotifier*>(this)->pageChanged();
    }

    /**
     * @brief previous
     *
     * Attempts to get the previous page and notifies pageChanged
     */
    auto previous() const -> void
    {
        setOffset(m_offset - m_resultsPerPage);
        Q_EMIT static_cast<PaginatorNotifier*>(this)->pageChanged();
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
        m_resultsPerPage = resultsPerPage;
        Q_EMIT static_cast<PaginatorNotifier*>(this)->resultsPerPageChanged();

        m_data = m_cacheController.chunk(m_offset, m_resultsPerPage);
        Q_EMIT static_cast<PaginatorNotifier*>(this)->dataChanged();
    }

    /**
     * @brief data
     * @return a copy of the list of pointers
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
    auto totalResults() -> qsizetype
    {
        return m_cacheController.count();
    }

    /**
     * @brief page
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
     * @return
     */
    auto totalPages() const -> qsizetype
    {
        return std::ceil(
            static_cast<qreal>(totalResults()) / m_resultsPerPage
        );
    }

protected:

    auto controller() -> SlidingCacheController<T>*
    {
        return &m_cacheController;
    }

    auto setOffset(qsizetype offset) -> void
    {
        m_offset = offset;

        if(m_offset >= m_totalResults)
        {
            m_offset = m_totalResults - m_resultsPerPage;
        }

        if(m_offset < 0)
        {
            m_offset = 0;
        }

        m_data = m_cacheController.chunk(m_offset, m_resultsPerPage);
        Q_EMIT this->countChanged();
        Q_EMIT this->dataChanged();
        Q_EMIT this->pageChanged();
        Q_EMIT this->totalPagesChanged();
    }

    auto onControllerTotalResultsChanged() -> void
    {
        Q_EMIT this->totalResultsChanged();
    }

private:
    qsizetype m_page = 0;
    qsizetype m_totalPages = 0;

    qsizetype m_offset = 0;
    qsizetype m_resultsPerPage = 0;
    qsizetype m_totalResults = 0;

    SlidingCacheController<T> m_cacheController;
    QList<T> m_data;
};

#endif // PAGINATOR_H
