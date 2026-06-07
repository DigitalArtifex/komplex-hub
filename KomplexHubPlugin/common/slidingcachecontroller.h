#ifndef SLIDINGCACHECONTROLLER_H
#define SLIDINGCACHECONTROLLER_H

#include <QObject>
#include <QList>
#include <functional>
#include "fetchresult.h"
#include "komplex_global.h"

template <typename T>
class KOMPLEX_EXPORT SlidingCacheController
{
public:

    explicit SlidingCacheController()
    {
        m_cache.reserve(SLIDING_CACHE_WINDOW_SIZE);
    }

    ~SlidingCacheController()
    {
        clear();
    }

    /**
     * @brief setFetch
     * Used to set the fetch method for the cache controller. Method should return a QList
     * of CacheObjects based on the offset and limit provided. CacheObject must derive from
     * QObject
     *
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
    auto at(qsizetype index) const -> T*
    {
        //OOB check for expected index
        if (index < 0 ||
            (m_totalCount != std::numeric_limits<qsizetype>::infinity() &&
             index >= m_totalCount))
        {
            return nullptr;
        }

        while (index >= m_windowOffset + (SLIDING_CACHE_WINDOW_SIZE - SLIDING_CACHE_PREFETCH_THRESHOLD))
        {
            if(!slideForward())
            {
                break;
            }
        }

        while (index <= m_windowOffset - (SLIDING_CACHE_WINDOW_SIZE - SLIDING_CACHE_PREFETCH_THRESHOLD))
        {
            if(!slideBackward())
            {
                break;
            }
        }

        index = trueIndex(index);

        //OOB check for actual index
        if(index < 0 || index >= m_cache.count())
        {
            return nullptr;
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
    auto chunk(qsizetype index, qsizetype count) -> QList<T*>
    {
        QList<T*> dataChunk;

        for(qsizetype i = index; i < count; ++i)
        {
            T* data = at(i);

            if(data == nullptr)
            {
                break;
            }

            dataChunk.append(data);
        }

        return dataChunk;
    }

    /**
     * @brief count
     *
     * @return The total number of results available
     */
    [[nodiscard]]
    auto count() const -> qsizetype
    {
        return m_totalCount;
    }

    /**
     * @brief clear
     * Clears the current cache
     */
    auto clear() -> void
    {
        while(!m_cache.isEmpty())
        {
            T *object = m_cache.takeFirst();
            delete object;
        }
    }

signals:

private:
    /**
     * @brief slideForward
     * Attempts to slide the cache window forward
     * @return true if successful
     */
    auto slideForward() -> bool
    {
        qsizetype movementSize = SLIDING_CACHE_WINDOW_SIZE;

        //after initial inflation we should only arrive here when the prefetch threshold is reached
        if(!m_cache.isEmpty())
        {
            movementSize -= SLIDING_CACHE_PREFETCH_THRESHOLD;
        }

        m_windowOffset += movementSize;

        QList<T*> newCache = m_fetch(
            m_windowOffset,
            movementSize
            );

        if(newCache.isEmpty())
        {
            m_windowOffset -= movementSize;
            return false;
        }

        while(!newCache.isEmpty() && !m_cache.isEmpty())
        {
            T *object = newCache.takeFirst();
            T *oldObject = m_cache.takeFirst();

            delete oldObject;
            m_cache.append(object);
        }

        if(!newCache.isEmpty())
        {
            LOG_ERROR_X((newCache.count() + m_cache.count()) > SLIDING_CACHE_WINDOW_SIZE,
                        "SlidingCacheController::slideForward",
                        "Cache size imbalance",
                        false
                        );

            m_cache.append(newCache);
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

        while(!result.data.isEmpty() && !m_cache.isEmpty())
        {
            T *object = result.data.takeLast();
            T *oldObject = m_cache.takeLast();

            delete oldObject;
            m_cache.push_front(object);
        }

        if(!result.results.isEmpty())
        {
            LOG_ERROR_X((result.count + m_cache.count()) > SLIDING_CACHE_WINDOW_SIZE,
                        "SlidingCacheController::slideBackward",
                        "Cache size imbalance",
                        false
                        );

            m_cache.push_front(std::move(result.data));
        }

        return true;
    }

    /**
     * @brief trueIndex
     * Translates the global object index to the current cache index
     * @param index Global object index
     * @return
     */
    auto trueIndex(qsizetype index) -> qsizetype
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
    QVector<T*> m_cache;
};

#endif // SLIDINGCACHECONTROLLER_H
