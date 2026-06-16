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
#ifndef SLIDINGCACHECONTROLLER_H
#define SLIDINGCACHECONTROLLER_H

#include <QObject>
#include <QList>
#include <QMutex>
#include <QMutexLocker>

#include <functional>
#include "fetchresult.h"
#include "komplex_global.h"
#include "logging.h"

/**
 * @brief The SlidingCacheNotifier class
 *
 * Qt does not support templated QObjects. Pushing it to a notifier class
 * that the template can inherit fixes this
 */
class KOMPLEX_EXPORT SlidingCacheNotifier : public QObject
{
    Q_OBJECT
signals:
    auto countChanged() -> void;
};

/**
 * @brief The SlidingCacheController class is a type and endpoint agnostic sliding window
 * cache controller. It pre-fetches a larger data set that a paginator works on top of.
 * This minimizes the number of API calls required to serve paged data
 */
template <typename T>
class KOMPLEX_EXPORT SlidingCacheController : public SlidingCacheNotifier
{
public:

    explicit SlidingCacheController() : SlidingCacheNotifier{}
    {
        m_cache.reserve(SLIDING_CACHE_WINDOW_SIZE);
    }

    ~SlidingCacheController()
    {
        clear();
    }

    /**
     * @brief setFetch
     *
     * Used to set the fetch method for the cache controller. Method should return a QList
     * of CacheObjects based on the offset and limit provided. CacheObject must derive from
     * QObject
     * @param fetch
     */
    auto setFetch(
        const std::function<FetchResult<T> (qsizetype offset, qsizetype limit)> &fetch
    ) -> void
    {
        m_fetch = std::move(fetch);
    }

    /**
     * @brief at
     * Retrieves the CacheObject reference at the specified index. If data is outside the
     * current window, a fetch will be attempted
     * @param index
     * @return CacheObject* or nullptr
     */
    [[nodiscard]]
    auto at(qsizetype index) const -> T
    {
        QMutexLocker cacheLocker(&m_cacheMutex);

        if(!externalBoundaryCheck(index))
        {
            return {};
        }

        if(!const_cast<SlidingCacheController*>(this)->slideWindow(index))
        {
            return {};
        }

        qsizetype internalIndex = this->internalIndex(index);

        if(!internalBoundaryCheck(index))
        {
            return {};
        }

        return m_cache.at(index);
    }

    /**
     * @brief chunk
     * Returns a chunk of data from cache. If data is outside the current window, a
     * fetch will be attempted
     * @param index
     * @param count
     * @return
     */
    [[nodiscard]]
    auto chunk(qsizetype index, qsizetype count) const -> QList<T>
    {
        QMutexLocker cacheLocker(&m_cacheMutex);

        if(!externalBoundaryCheck(index))
        {
            return {};
        }

        if(!const_cast<SlidingCacheController*>(this)->slideWindow(index))
        {
            return {};
        }

        qsizetype internalIndex = this->internalIndex(index);

        if(!internalBoundaryCheck(index))
        {
            return {};
        }

        if(!internalBoundaryCheck(count - 1))
        {
            count = m_cache.count() - index;
        }

        QList<T> dataChunk;

        for(qsizetype i = index; i < count; ++i)
        {
            T data = m_cache.at(i);

            if(data != T{})
            {
                dataChunk.append(data);
            }
        }

        return dataChunk;
    }

    /**
     * @brief totalCount
     *
     * @return The total number of results available
     */
    [[nodiscard]]
    auto totalCount() const -> qsizetype
    {
        if(m_totalCount == std::numeric_limits<qsizetype>::infinity())
        {
            return 0;
        }

        return m_totalCount;
    }

    /**
     * @brief clear
     * Clears the current cache
     */
    auto clear() -> void
    {
        QMutexLocker cacheLocker(&m_cacheMutex);

        m_cache.clear();
    }

    /**
     * @brief reset
     * Resets the current cache model
     */
    auto reset() -> void
    {
        QMutexLocker cacheLocker(&m_cacheMutex);

        m_cache.clear();
        setTotalCount(std::numeric_limits<qsizetype>::infinity());
        m_windowOffset = 0;
    }

protected:
    /**
     * @brief setTotalCount
     * Sets the total count and notifies countChanged
     * @param count
     */
    auto setTotalCount(qsizetype count) -> void
    {
        if(count == m_totalCount)
            return;

        m_totalCount = count;
        Q_EMIT static_cast<SlidingCacheNotifier*>(this)->countChanged();
    }

private:
    /**
     * @brief externalBoundaryCheck
     *
     * Checks if the index is within the known bounds of the external
     * dataset
     * @param index
     * @return True if index is within the external dataset boundary
     * OR an initial fetch has not been performed.
     */
    auto externalBoundaryCheck(qsizetype index) const -> bool
    {
        return (
            index >= 0 &&
            (
                m_totalCount == std::numeric_limits<qsizetype>::infinity() ||
                index < m_totalCount
            )
        );
    }

    /**
     * @brief internalBoundaryCheck
     *
     * Checks if the index is within the currently cached dataset
     * @param index
     * @return True if index is within the internal dataset boundary
     */
    auto internalBoundaryCheck(qsizetype index) const -> bool
    {
        return (
            index >= 0 &&
            index < m_cache.count()
        );
    }

    /**
     * @brief slideWindow
     *
     * Slides the current data window to the provided index
     * @param index
     * @return
     */
    auto slideWindow(qsizetype index) -> bool
    {
        if (!externalBoundaryCheck(index))
        {
            return false;
        }

        while (index >= m_windowOffset + (SLIDING_CACHE_WINDOW_SIZE - SLIDING_CACHE_PREFETCH_THRESHOLD))
        {
            if(!slideForward())
            {
                return false;
            }
        }

        while (index <= m_windowOffset - (SLIDING_CACHE_WINDOW_SIZE - SLIDING_CACHE_PREFETCH_THRESHOLD))
        {
            if(!slideBackward())
            {
                return false;
            }
        }

        if(m_totalCount == std::numeric_limits<qsizetype>::infinity())
        {
            if(!slideForward())
            {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief slideForward
     * Attempts to slide the cache window forward
     * @return true if successful
     */
    auto slideForward() -> bool
    {
        LOG_ERROR_X(!m_fetch,
            "SlidingCacheController::slideBackward",
            "Fetch function not set",
            false
        );

        qsizetype movementSize = SLIDING_CACHE_WINDOW_SIZE;

        //after initial inflation we should only arrive here when the prefetch threshold is reached
        if(!m_cache.isEmpty())
        {
            movementSize -= SLIDING_CACHE_PREFETCH_THRESHOLD;
        }

        if(m_totalCount != std::numeric_limits<qsizetype>::infinity())
        {
            m_windowOffset += movementSize;
        }
        else
        {
            m_windowOffset = 0;
        }

        FetchResult<T> result = m_fetch(
            m_windowOffset,
            movementSize
        );

        if(result.count == 0)
        {
            m_windowOffset += movementSize;
            return false;
        }

        setTotalCount(result.total);

        if(result.data.isEmpty())
        {
            m_windowOffset -= movementSize;
            return false;
        }

        while(!result.data.isEmpty() && !m_cache.isEmpty())
        {
            T object = result.data.takeFirst();
            m_cache.removeFirst();

            m_cache.append(std::move(object));
        }

        if(!result.data.isEmpty())
        {
            LOG_ERROR_X((result.data.count() + m_cache.count()) > SLIDING_CACHE_WINDOW_SIZE,
                "SlidingCacheController::slideForward",
                "Cache size imbalance",
                false
            );

            m_cache.append(std::move(result.data));
        }

        return true;
    }

    /**
     * @brief slideBackward
     * Attempts to slide the cache window backward
     * @return true if successful
     */
    auto slideBackward() -> bool
    {
        LOG_ERROR_X(!m_fetch,
            "SlidingCacheController::slideBackward",
            "Fetch function not set",
            false
        );

        qsizetype movementSize = SLIDING_CACHE_WINDOW_SIZE - SLIDING_CACHE_PREFETCH_THRESHOLD;
        m_windowOffset -= movementSize;

        if(m_windowOffset < 0)
        {
            m_windowOffset = 0;
        }

        FetchResult<T> result = m_fetch(
            m_windowOffset,
            movementSize
        );

        if(result.count == 0)
        {
            m_windowOffset += movementSize;
            return false;
        }

        setTotalCount(result.total);

        while(!result.data.isEmpty() && !m_cache.isEmpty())
        {
            T object = result.data.takeLast();
            m_cache.removeLast();

            m_cache.push_front(std::move(object));
        }

        if(!result.data.isEmpty())
        {
            LOG_ERROR_X((result.count + m_cache.count()) > SLIDING_CACHE_WINDOW_SIZE,
                "SlidingCacheController::slideBackward",
                "Cache size imbalance",
                false
            );

            while(!result.data.isEmpty())
            {
                T object = result.data.takeLast();
                m_cache.push_front(std::move(object));
            }
        }

        return true;
    }

    /**
     * @brief internalIndex
     * Translates the global object index to the current cache index
     * @param index Global object index
     * @return
     */
    auto internalIndex(qsizetype index) const -> qsizetype
    {
        return index - m_windowOffset;
    }

    /**
     * @brief m_windowOffset
     * Current window offset
     */
    qsizetype m_windowOffset = 0;

    /**
     * @brief m_totalCount
     * Total number of results available. Data comes from the return data of
     * m_fetch
     */
    qsizetype m_totalCount = std::numeric_limits<qsizetype>::infinity();

    /**
     * @brief m_fetch
     * The fetch function for retreiving external data. This must be set by the paginator
     * or other controller class for the cachecontroller to work
     */
    std::function<FetchResult<T>(qsizetype offset, qsizetype limit)> m_fetch;

    /**
     * @brief m_cache
     * The working cache
     */
    QVector<T> m_cache;

    /**
     * @brief m_cacheMutex
     */
    mutable QMutex m_cacheMutex;
};

#endif // SLIDINGCACHECONTROLLER_H
