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
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <QString>
#include <string>
#include <filesystem>
#include "komplex_global.h"

struct KOMPLEX_EXPORT network_exception : std::ios_base::failure
{
    network_exception(const std::string &message, const std::string &details, const std::error_code &code) :
        details(details),
        std::ios_base::failure(message, code) { }

    network_exception(const std::string &message, const std::error_code &code) :
        std::ios_base::failure(message, code) { }

    network_exception(const QString &message, const QString &details, const std::error_code &code) :
        details(details.toStdString()),
        std::ios_base::failure(message.toStdString(), code) { }

    network_exception(const QString &message, const int code = errno) :
        std::ios_base::failure
        (
            message.toStdString(),
            std::error_code
            (
                code,
                std::system_category()
            )
        ) { }

    const std::string details;
};

struct KOMPLEX_EXPORT file_exception : std::filesystem::filesystem_error
{
    file_exception(const std::string &message, const std::string &details, const std::error_code &code) :
        details(details),
        std::filesystem::filesystem_error(message, code) { }

    file_exception(const std::string &message, const std::error_code &code) :
        std::filesystem::filesystem_error(message, code) { }

    file_exception(const std::string &message, const int code = errno) :
        std::filesystem::filesystem_error
        (
            message,
            std::error_code
            (
                code,
                std::system_category()
            )
        ) { }

    file_exception(const std::string &message, const std::string &details, const int code = errno) :
        details(details),
        std::filesystem::filesystem_error
        (
            message,
            std::error_code
            (
                code,
                std::system_category()
            )
        ) { }

    const std::string details;
};

struct KOMPLEX_EXPORT sql_exception : std::ios_base::failure
{
    sql_exception(const std::string &message, const std::string &details, const std::error_code &code) :
        details(details),
        std::ios_base::failure(message, code) { }

    sql_exception(const std::string &message, const std::error_code &code) :
        std::ios_base::failure(message, code) { }

    sql_exception(const std::string &message, const int code = errno) :
        std::ios_base::failure
        (
            message,
            std::error_code
            (
                code,
                std::system_category()
            )
        ) { }

    const std::string details;
};

struct KOMPLEX_EXPORT wallet_exception : std::ios_base::failure
{
    wallet_exception(const std::string &message, const std::string &details, const std::error_code &code) :
        details(details),
        std::ios_base::failure(message, code) { }

    wallet_exception(const std::string &message, const std::error_code &code) :
        std::ios_base::failure(message, code) { }

    wallet_exception(const std::string &message, const std::string &details, const int code = errno) :
        details(details),
        std::ios_base::failure
        (
            message,
            std::error_code
            (
                code,
                std::system_category()
            )
        ) { }

    wallet_exception(const std::string &message, const int code = errno) :
        std::ios_base::failure
        (
            message,
            std::error_code
            (
                code,
                std::system_category()
            )
        ) { }

    const std::string details;
};

struct KOMPLEX_EXPORT process_exception : std::runtime_error
{
    process_exception(const std::string &message, const std::string &details, const std::error_code &code) :
        details(details),
        code(code),
        std::runtime_error(message) { }

    process_exception(const std::string &message, const std::error_code &code) :
        code(code),
        std::runtime_error(message) { }

    process_exception(const std::string &message, const int code = errno) :
        code
        (
            std::error_code
            (
                code,
                std::system_category()
            )
        ),
        std::runtime_error
        (
            message
        ) { }

    process_exception(const std::string &message, const std::string &details, const int code = errno) :
        details(details),
        code
        (
            std::error_code
            (
                code,
                std::system_category()
            )
        ),
        std::runtime_error (message) { }

    const std::string details;
    const std::error_code code;
};

namespace shader
{
    struct KOMPLEX_EXPORT logic_error : std::logic_error
    {
        logic_error(const std::string &message, const std::string &details, const std::error_code &code) :
            details(details),
            code(code),
            std::logic_error(message) { }

        logic_error(const std::string &message, const std::error_code &code) :
            code(code),
            std::logic_error(message) { }

        logic_error(const std::string &message, const std::string &details = {}, const int code = errno) :
            code
            (
                std::error_code
                (
                    code,
                    std::system_category()
                )
            ),
            std::logic_error(message) { }

        const std::string details;
        const std::error_code code;
    };
}
#endif // EXCEPTIONS_H
