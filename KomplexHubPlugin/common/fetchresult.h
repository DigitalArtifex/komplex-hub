#ifndef FETCHRESULT_H
#define FETCHRESULT_H
#include <qtypes.h>
#include <QList>

template<typename T>
struct FetchResult
{
    /**
     * @brief total
     * Total number of results in the query
     */
    qsizetype total = 0;

    /**
     * @brief count
     * Number of results returned in this result
     */
    qsizetype count = 0;

    /**
     * @brief results
     * Results list
     */
    QList<T*> data;
};

#endif // FETCHRESULT_H
