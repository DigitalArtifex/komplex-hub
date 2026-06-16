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
    QList<T> data;

    auto operator=(const FetchResult<T> &other) -> FetchResult<T>
    {
        total = other.total;
        count = other.count;
        data = other.data;
    }
};

#endif // FETCHRESULT_H
