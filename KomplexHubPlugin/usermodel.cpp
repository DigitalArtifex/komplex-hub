#include "usermodel.h"
#include "common/userservices.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <qdatetime.h>
#include <qfuture.h>
#include <qobjectdefs.h>
#include <qstringview.h>

UserModel::UserModel(QObject *parent)
    : QObject{parent}
{
    m_stateResetTimer = new QTimer(this);
    m_stateResetTimer->setInterval(1000);
    m_stateResetTimer->setSingleShot(true);

    QObject::connect
    (
        m_stateResetTimer,
        &QTimer::timeout,
        this,
        &UserModel::resetState
    );

    setState(Busy);

    QSharedPointer<UserServices> service = UserServices::instance();

    if(service == nullptr)
    {
        setErrorString(QStringLiteral("UserServices object deleted"));
        setState(LoginFailure);
        return;
    }

    QFuture<QString> usernameFuture = service->defaultUsername();

    usernameFuture
        .then
        (
            [this] (const QString username)
            {
                login(username);
            }
        )
        .onCanceled
        (
            [this] () -> void
            {
                reset();
            }
        )
        .onFailed
        (
            [this] () -> void
            {
                reset();
            }
        );

    service.clear();
}

UserModel::~UserModel()
{
    if(m_stateResetTimer != nullptr)
    {
        m_stateResetTimer->deleteLater();
    }
}

auto UserModel::login() -> void
{
    QFuture aether = QtConcurrent::run
    (
        [this] () -> void
        {
            QMutexLocker locker(&m_loginMutex);
            setState(LoggingIn);

            QSharedPointer<UserServices> service = UserServices::instance();

            if(service == nullptr)
            {
                setErrorString(QStringLiteral("UserServices object deleted"));
                setState(LoginFailure);
                return;
            }

            m_loginAttempt = service->login(username(), password().toUtf8());

            m_loginAttempt
                .then
                (
                    [this] (UserCredentials credentials) -> void
                    {
                        setState(LoggedIn);
                    }
                )
                .onCanceled
                (
                    [this] () -> void
                    {
                        setState(LoginFailure);
                    }
                )
                .onFailed
                (
                    [this] () -> void
                    {
                        setState(LoginCancelled);
                    }
                );
        }
    );
}

auto UserModel::login(const QString username) -> void
{
    if(username == m_username)
    {
        return;
    }

    m_username = username;
    Q_EMIT usernameChanged();

    QSharedPointer<UserServices> service = UserServices::instance();

    if(service == nullptr)
    {
        setErrorString(QStringLiteral("UserServices object deleted"));
        setState(LoggedOut);
        return;
    }

    QFuture<UserCredentials> tokenRead = service->readToken(username);

    tokenRead
        .then
        (
            [this] (const UserCredentials credentials) -> void
            {
                if(QDateTime::currentDateTime() <= credentials.expiry)
                {
                    setState(LoggedIn);
                    return;
                }

                QSharedPointer<UserServices> service = UserServices::instance();

                if(service == nullptr)
                {
                    setErrorString(QStringLiteral("UserServices object deleted"));
                    setState(LoggedOut);
                    return;
                }

                QFuture<QByteArray> passwordRead = service->readPassword(credentials.username);

                passwordRead
                    .then
                    (
                        [this] (const QByteArray password)
                        {
                            if(password.isEmpty())
                            {
                                setState(LoggedOut);
                                return;
                            }

                            setPassword(password);
                        }
                    );

                service.clear();
            }
        );

    service.clear();
}

auto UserModel::cancelLogin() -> void
{
    QFuture aether = QtConcurrent::run
    (
        [this] () -> void
        {
            QMutexLocker locker(&m_loginMutex);

            if(m_loginAttempt.isRunning())
            {
                m_loginAttempt.cancel();
            }
        }
    );
}

auto UserModel::reset() -> void
{
    resetState();
    setUsername({});
    setPassword({});
}

auto UserModel::setUsername(const QString &username) -> void
{
    if(username == m_username)
    {
        return;
    }

    m_username = username;
    Q_EMIT usernameChanged();

    if(!password().isEmpty())
    {
        return;
    }

    setState(Busy);

    QSharedPointer<UserServices> service = UserServices::instance();

    if(service == nullptr)
    {
        return;
    }

    QFuture<QByteArray> passwordRead = service->readPassword(username);
    passwordRead
        .then
        (
            [this] (const QByteArray password) -> void
            {
                setPassword(password);
            }
        );
}

auto UserModel::setPassword(const QString &password) -> void
{
    if(password == m_password)
    {
        return;
    }

    m_password = password;
    Q_EMIT passwordChanged();
}

auto UserModel::resetState() -> void
{
    setState(Idle);
}

auto UserModel::setState(State state) -> void
{
    if(state == m_state)
    {
        return;
    }

    switch(state)
    {
    case LoginFailure:
    case LoggedOut:
    case LoginCancelled:
        m_stateResetTimer->start();
        break;

    default:
        break;
    }

    m_state = state;
    Q_EMIT stateChanged();
}

auto UserModel::setErrorString(const QString &string) -> void
{
    if(string == m_errorString)
    {
        return;
    }

    m_errorString = string;
    Q_EMIT errorStringChanged();
}
