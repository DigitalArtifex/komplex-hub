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
#include <QThread>

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
    auto fetching() -> void;
    auto fetchComplete() -> void;
    auto windowSizeChanged() -> void;
};

/**
 * @brief The SlidingCacheController class is a type and endpoint agnostic sliding window
 * cache controller. It pre-fetches a larger data set that a paginator works on top of.
 * This minimizes the number of API calls required to serve paged data
 */
template <typename T>
class KOMPLEX_EXPORT SlidingCacheController : public SlidingCacheNotifier
{
    /**
     * @brief FetchFunction
     * Convenience def used by setFetch
     */
    typedef std::function<FetchResult<T> (qsizetype offset, qsizetype limit)> FetchFunction;

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
    auto setFetch(const FetchFunction &fetch) -> void
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
    auto at(qsizetype index) const -> const T&
    {
        QMutexLocker cacheLocker(&m_cacheMutex);

        if(!externalBoundaryCheck(index))
        {
            return defaultValue;
        }

        if(!const_cast<SlidingCacheController*>(this)->slideWindow(index))
        {
            return defaultValue;
        }

        qsizetype internalIndex = this->internalIndex(index);

        if(!internalBoundaryCheck(internalIndex))
        {
            return defaultValue;
        }

        return m_cache.at(internalIndex);
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

        if(!internalBoundaryCheck(internalIndex))
        {
            return {};
        }

        if(!internalBoundaryCheck(count - 1))
        {
            count = m_cache.count() - internalIndex;
        }

        return m_cache.mid(internalIndex, count);
    }

    /**
     * @brief totalCount
     *
     * @return The total number of results available
     */
    [[nodiscard]]
    auto totalCount() const -> qsizetype
    {
        if(m_totalCount == nullsize)
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
        setTotalCount(nullsize);
        m_windowOffset = 0;
    }

    auto focus(qsizetype index, qsizetype count) -> void
    {
        if(inFocus(index, count) || !externalBoundaryCheck(index))
        {
            return;
        }

        if(externalBoundaryCheck(index + count))
        {
            slideWindow(index);
        }
    }

    auto init() -> bool
    {
        if(m_totalCount != nullsize)
        {
            return false;
        }

        return slideForward();
    }

    auto windowSize() const -> qsizetype
    {
        if(m_windowSize == nullsize)
        {
            return SLIDING_CACHE_WINDOW_SIZE;
        }

        return m_windowSize;
    }

    auto setWindowSize(qsizetype windowSize) -> void
    {
        if(windowSize == m_windowSize)
        {
            return;
        }

        m_windowSize = windowSize;
        Q_EMIT windowSizeChanged();
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
    auto inFocus(qsizetype index, qsizetype count) -> bool
    {
        qsizetype internalIndex = this->internalIndex(index);

        return
        (
            internalBoundaryCheck(internalIndex, internalIndex + count)
        );
    }

    /**
     * @brief externalBoundaryCheck
     *
     * Checks if the index is within the known bounds of the external
     * dataset
     * @param index
     * @return True if index is within the external dataset boundary
     * OR an initial fetch has not been performed.
     */
    template<typename... Ts>
    auto externalBoundaryCheck(Ts... index) const -> bool
    {
        static_assert
        (
            (std::is_same_v<Ts, qsizetype> && ...),
            "All arguments must be qsizetype"
        );

        bool safe = false;

        ((safe = externalBoundaryCheckHelper(index)),...);

        return safe;
    }

    auto externalBoundaryCheckHelper(qsizetype index) const -> bool
    {
        return
        (
            index >= 0 &&
            (
                m_totalCount == nullsize ||
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
    template<typename... Ts>
    auto internalBoundaryCheck(Ts... index) const -> bool
    {
        static_assert
        (
            (std::is_same_v<Ts, qsizetype> && ...),
            "All arguments must be qsizetype"
        );

        bool safe = false;
        ((safe = ((index >= 0) && (index < m_cache.count()))), ...);
        return safe;
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

        qsizetype prefetchThreshold = SLIDING_CACHE_PREFETCH_THRESHOLD;

        if(prefetchThreshold >= windowSize())
        {
            prefetchThreshold = std::floor
            (
                static_cast<qreal>(windowSize()) * 0.8
            );
        }

        while (index >= m_windowOffset + (windowSize() - prefetchThreshold))
        {
            if(!slideForward())
            {
                return false;
            }
        }

        while (index < m_windowOffset)
        {
            if(!slideBackward())
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
            "SlidingCacheController::slideForward",
            "Fetch function not set",
            false
        );

        Q_EMIT fetching();

        qsizetype movementSize = SLIDING_CACHE_WINDOW_SIZE;
        qsizetype prefetchThreshold = SLIDING_CACHE_PREFETCH_THRESHOLD;

        if(m_windowSize != nullsize)
        {
            movementSize = m_windowSize;

            if(prefetchThreshold >= m_windowSize)
            {
                prefetchThreshold = std::floor(
                    static_cast<qreal>(movementSize) * 0.8
                );
            }
        }

        LOG_ERROR_X(movementSize <= 0 || prefetchThreshold <= 0,
            "SlidingCacheController::slideForward",
            "Invalid movement size",
            false
        );

        //after initial inflation we should only arrive here when the prefetch threshold is reached
        if(!m_cache.isEmpty())
        {
            movementSize -= prefetchThreshold;
        }

        if(m_totalCount != nullsize)
        {
            m_windowOffset += movementSize;
        }
        else
        {
            m_windowOffset = 0;
        }

        // QThread::sleep(KOMPLEX_RATE_LIMIT);

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

            Q_EMIT fetchComplete();
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

        Q_EMIT fetchComplete();

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

        Q_EMIT fetching();

        qsizetype movementSize = SLIDING_CACHE_WINDOW_SIZE;
        qsizetype prefetchThreshold = SLIDING_CACHE_PREFETCH_THRESHOLD;

        if(m_windowSize != nullsize)
        {
            movementSize = m_windowSize;

            if(prefetchThreshold >= m_windowSize)
            {
                prefetchThreshold = std::floor(
                    static_cast<qreal>(movementSize) * 0.8
                );
            }
        }

        LOG_ERROR_X(movementSize <= 0 || prefetchThreshold <= 0,
            "SlidingCacheController::slideBackward",
            "Invalid movement size",
            false
        );

        // QThread::sleep(KOMPLEX_RATE_LIMIT);

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

            Q_EMIT fetchComplete();
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

        Q_EMIT fetchComplete();

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
    qsizetype m_totalCount = nullsize;

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

    /**
     * @brief m_windowSize
     * Override for window size
     */
    qsizetype m_windowSize = SLIDING_CACHE_WINDOW_SIZE;

    T defaultValue;
};

#endif // SLIDINGCACHECONTROLLER_H
