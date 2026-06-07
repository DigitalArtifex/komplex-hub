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
#ifndef LOGGING_H
#define LOGGING_H
// clazy:exclude=clazy-unused-headers

#include <iostream> // NOLINT(clazy-unused-headers)
#include <cstdlib> // NOLINT(clazy-unused-headers)

/**
 * @brief LOG_ERROR
 *
 * When cond is true, logs where and what as an error
 * @param cond
 * @param where
 * @param what
 */
#define LOG_ERROR(where, what)                                           \
    do {                                                                 \
            std::cerr << "Error Occurred:\n"                             \
                << "  Location: " << (where) << "\n"                     \
                << "  Message: " << (what) << "\n"                       \
                << "  File: " << __FILE__ << ":" << __LINE__ << "\n";    \
} while (0)

/**
 * @brief LOG_ERROR_X
 *
 * When cond is true, logs where and what as an error and returns the ret value
 * @param cond
 * @param where
 * @param what
 * @param ret
 */
#define LOG_ERROR_X(cond, where, what, ret)                              \
    do {                                                                 \
        if ((cond)) {                                                    \
            std::cerr << "Error Occurred: (" << #cond << ")\n"           \
                << "  Location: " << (where) << "\n"                     \
                << "  Message: " << (what) << "\n"                       \
                << "  File: " << __FILE__ << ":" << __LINE__ << "\n";    \
            return ret;                                                  \
    }                                                                    \
} while (0)

#ifndef ASSERT_X
#define ASSERT_X(cond, where, what)                                      \
    do {                                                                 \
        if (!(cond)) {                                                   \
            std::cerr << "Assertion failed: (" << #cond << ")\n"         \
                << "  Location: " << (where) << "\n"                     \
                << "  Message: " << (what) << "\n"                       \
                << "  File: " << __FILE__ << ":" << __LINE__ << "\n";    \
            std::abort();                                                \
    }                                                                    \
} while (0)
#endif
#endif // LOGGING_H
