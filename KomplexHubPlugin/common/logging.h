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
#define LOG_ERROR(cond, where, what)                                     \
    do {                                                                 \
        if ((cond)) {                                                    \
            std::cerr << "Assertion failed: (" << #cond << ")\n"         \
                << "  Location: " << (where) << "\n"                     \
                << "  Message: " << (what) << "\n"                       \
                << "  File: " << __FILE__ << ":" << __LINE__ << "\n";    \
        }                                                                \
} while (0)

#define LOG_ERROR_X(cond, where, what, ret)                              \
    do {                                                                 \
        if ((cond)) {                                                    \
            std::cout << "Assertion failed: (" << #cond << ")\n"         \
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
            std::cout << "Assertion failed: (" << #cond << ")\n"         \
                << "  Location: " << (where) << "\n"                     \
                << "  Message: " << (what) << "\n"                       \
                << "  File: " << __FILE__ << ":" << __LINE__ << "\n";    \
            std::abort();                                                \
    }                                                                    \
} while (0)
#endif
#endif // LOGGING_H
