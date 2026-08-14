#pragma once

#include <QGuiApplication>
#include <QObject>
#include <QDateTime>
#include <QSharedPointer>
#include <QFuture>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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

class KOMPLEX_EXPORT UserServices : public QObject
{
    Q_OBJECT

public:
    auto instance() const -> QWeakPointer<UserServices>;
    static auto userCredentials() -> const UserCredentials &;
    auto login(const QString &username, const QByteArray &password) -> QFuture<UserCredentials>;

    auto errorString() const -> const QString &;

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

    QWallet *m_wallet = nullptr;
    QString m_errorString;
    inline static UserCredentials s_userCredentials;
    Q_PROPERTY(UserCredentials userCredentials READ userCredentials NOTIFY userCredentialsChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
};