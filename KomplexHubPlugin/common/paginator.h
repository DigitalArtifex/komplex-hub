#ifndef PAGINATOR_H
#define PAGINATOR_H

#include <QObject>

#include "komplex_global.h"
#include "slidingcachecontroller.h"

template<typename T>
class KOMPLEX_EXPORT Paginator
{
public:
    Paginator() = default;
    ~Paginator() = default;

    auto next() const -> void
    {
        setOffset(m_offset + m_resultsPerPage);
    }

    auto previous() const -> void
    {
        setOffset(m_offset - m_resultsPerPage);
    }

    auto resultsPerPage() const -> qsizetype
    {
        return m_resultsPerPage;
    }

    auto setResultsPerPage(qsizetype resultsPerPage) -> void
    {
        m_resultsPerPage = resultsPerPage;
        m_data = m_cacheController.chunk(m_offset, m_resultsPerPage);
    }

    auto data() const -> QList<T*>
    {
        return m_data;
    }

    auto count() const -> qsizetype
    {
        return m_data.count();
    }

    auto at(qsizetype index) const -> T*
    {
        if(index < 0 || index >= m_data.count())
        {
            return nullptr;
        }

        return m_data.at(index);
    }

    auto controller() -> SlidingCacheController<T>*
    {
        return &m_cacheController;
    }

    auto offset() const -> qsizetype
    {
        return m_offset;
    }

    auto setOffset(qsizetype offset) -> void
    {
        m_offset = offset;

        if(m_offset < 0 || m_offset == std::numeric_limits<qsizetype>::max())
            m_offset = 0;

        m_data = m_cacheController.chunk(m_offset, m_resultsPerPage);
    }

private:
    qsizetype m_offset = 0;
    qsizetype m_resultsPerPage = 0;

    SlidingCacheController<T> m_cacheController;
    QList<T*> m_data;
};

#endif // PAGINATOR_H
