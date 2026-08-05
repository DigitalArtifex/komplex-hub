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

struct NetworkException : Exception
{
    NetworkException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
struct FileException : Exception
{
    FileException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
struct SqlException : Exception
{
    SqlException(const QString &message, qsizetype errorCode = 0) : Exception(message, errorCode) {}
};
#endif // EXCEPTIONS_H
