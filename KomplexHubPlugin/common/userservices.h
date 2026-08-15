#pragma once

#include <QGuiApplication>
#include <QObject>
#include <QDateTime>
#include <QSharedPointer>
#include <QFuture>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "komplex_global.h"
#include "qwallet.h"

struct UserCredentials
{
    QByteArray username;
    QByteArray sessionToken;
    QDateTime expiry;

    UserCredentials &operator = (const UserCredentials &other)
    {
        username = other.username;
        sessionToken = other.sessionToken;
        expiry = other.expiry;

        return *this;
    }

    bool isValid()
    {
        return !username.isEmpty() && username.isValidUtf8() &&
               !sessionToken.isEmpty() && sessionToken.isValidUtf8();
    }
};
Q_DECLARE_METATYPE(UserCredentials)

class KOMPLEX_EXPORT UserServices : public QObject
{
    Q_OBJECT

public:
    static auto instance() -> QWeakPointer<UserServices>;
    static auto userCredentials() -> const UserCredentials &;

    [[nodiscard]]
    auto login(const QString &username, const QByteArray &password) -> QFuture<UserCredentials>;

    auto errorString() const -> const QString &;

    [[nodiscard]]
    auto readPassword(const QString &username) -> QFuture<QByteArray>;

    [[nodiscard]]
    auto readToken(const QString &username) -> QFuture<UserCredentials>;

    [[nodiscard]]
    auto defaultUsername() -> QFuture<QString>;

signals:
    auto loginComplete() -> void;
    auto loginFailed() -> void;
    auto userCredentialsChanged() -> void;
    auto errorStringChanged() -> void;

private:
    inline static UserServices *s_instance = nullptr;
    inline static QSharedPointer<UserServices> s_pointer;

    explicit UserServices(QObject *parent);
    ~UserServices();

    auto setUserCredentials(const UserCredentials &credentials) -> void;
    auto setErrorString(const QString &errorString) -> void;
    auto savePassword(const QString &username, const QByteArray &password) -> void;
    auto saveToken(const QString &username, const QByteArray &token, const QDateTime &expiry) -> void;
    auto saveDefaultUsername(const QString &username) -> void;

    QWallet *m_wallet = nullptr;
    QString m_errorString;
    inline static UserCredentials s_userCredentials;

    inline const static char PART_SEPARATOR = static_cast<char>('\0x1f');

    Q_PROPERTY(UserCredentials userCredentials READ userCredentials NOTIFY userCredentialsChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
};
Q_DECLARE_METATYPE(UserServices)