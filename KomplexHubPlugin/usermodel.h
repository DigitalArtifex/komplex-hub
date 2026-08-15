#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QMutexLocker>
#include <qqmlintegration.h>
#include "common/userservices.h"

class UserModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:

    enum State
    {
        Idle,
        LoggingIn,
        LoggedIn,
        LoginFailure,
        LoginCancelled,
        LoggedOut,
        Busy
    };
    Q_ENUM(State)

    explicit UserModel(QObject *parent = nullptr);
    ~UserModel();

    Q_INVOKABLE auto login() -> void;
    Q_INVOKABLE auto login(const QString username) -> void;
    Q_INVOKABLE auto cancelLogin() -> void;
    Q_INVOKABLE auto reset() -> void;

    auto username() const -> const QString & { return m_username; }
    auto setUsername(const QString &username) -> void;

    auto password() const -> const QString & { return m_password; }
    auto setPassword(const QString &password) -> void;

    auto state() const -> State { return m_state; }
    auto errorString() const -> QString { return m_errorString; }

signals:
    auto loginSuccessful() -> void;
    auto loginFailed() -> void;
    auto stateChanged() -> void;
    auto usernameChanged() -> void;
    auto passwordChanged() -> void;
    auto errorStringChanged() -> void;

protected slots:
    auto resetState() -> void;

private:
    auto setState(State state) -> void;
    auto setErrorString(const QString &string) -> void;

    State m_state = Idle;
    QString m_errorString;
    QString m_username;
    QString m_password;
    QTimer *m_stateResetTimer = nullptr;
    QMutex m_loginMutex;
    QFuture<UserCredentials> m_loginAttempt;

    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
};

#endif // USERMODEL_H
