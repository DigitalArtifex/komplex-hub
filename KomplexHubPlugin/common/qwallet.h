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
#ifndef QWALLET_H
#define QWALLET_H

#include <QObject>
#include <QFuture>
#include <QEventLoop>
#include <QtConcurrent/QtConcurrentRun>

#include "komplex_global.h"

/**
 * @brief The QWallet class
 * QWallet is a QFuture wrapper for QtKeychain to make interaction with the keychain easier
 */
class KOMPLEX_EXPORT QWallet : public QObject
{
    Q_OBJECT

public:
    explicit QWallet(QObject *parent = nullptr);
    ~QWallet();

    /**
     * @brief read
     * Reads the provided key from the system keychain/wallet
     * @param service
     * Service name of the application making the request
     * @param key
     * Key to read
     * @return
     * QFuture for the binary value
     */
    [[nodiscard]]
    auto read(const QString &service, const QString &key) const -> QFuture<QByteArray>;

    /**
     * @brief write
     * Writes the provided value to the system kaychain/wallet
     * @param service
     * Service name of the application making the request
     * @param key
     * Key to read
     * @param value
     * Binary data to store
     * @return
     * QFuture
     */
    [[nodiscard]]
    auto write(const QString &service, const QString &key, const QByteArray &value) -> QFuture<void>;

    /**
     * @brief write
     * Removes the provided value from the system kaychain/wallet
     * @param service
     * Service name of the application making the request
     * @param key
     * Key to read
     * @return
     */
    [[nodiscard]]
    auto remove(const QString &service, const QString &key) -> QFuture<void>;

    /**
     * @brief errorString
     * String describing the last encountered error.
     * @return
     */
    [[nodiscard]]
    auto errorString() const -> const QString &;

private:
    /**
     * @brief setErrorString
     * Sets the error string
     * @param errorString
     */
    auto setErrorString(const QString &errorString) const -> void;

    mutable QString m_errorString;
};

#endif // QWALLET_H
