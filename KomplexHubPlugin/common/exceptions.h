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
#include <exception>
#include "komplex_global.h"

struct KOMPLEX_EXPORT Exception : std::exception
{
    explicit Exception(const QString &message, quint16 errorCode = 0)
        : std::exception(), message(message), errorCode(errorCode){}

    const QString message;
    const quint16 errorCode;
};

struct KOMPLEX_EXPORT NetworkException : Exception
{
    NetworkException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
struct KOMPLEX_EXPORT FileException : Exception
{
    FileException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
struct KOMPLEX_EXPORT SqlException : Exception
{
    SqlException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
struct KOMPLEX_EXPORT WalletException : Exception
{
    WalletException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
struct KOMPLEX_EXPORT ShaderCompilerException : Exception
{
    ShaderCompilerException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
#endif // EXCEPTIONS_H
