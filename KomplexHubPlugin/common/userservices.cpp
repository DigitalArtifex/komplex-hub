#include "userservices.h"
#include "coreservices.h"

UserServices::UserServices(QObject *parent) : QObject(parent)
{
    m_wallet = new QWallet(this);
}

UserServices::~UserServices()
{
}

auto UserServices::setUserCredentials(const UserCredentials &credentials) -> void
{
    s_userCredentials = credentials;
    Q_EMIT userCredentialsChanged();
}

auto UserServices::errorString() const -> const QString &
{
    return m_errorString;
}

auto UserServices::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
        return;
    m_errorString = errorString;
    emit errorStringChanged();
}

auto UserServices::instance() const -> QWeakPointer<UserServices>
{
    if(!s_pointer)
    {
        s_instance = new UserServices(QCoreApplication::instance());
        s_pointer = QSharedPointer<UserServices>
        (
            s_instance,
            &UserServices::deleteLater
        );
    }

    return s_pointer.toWeakRef();
}

auto UserServices::userCredentials() -> const UserCredentials &
{
    return s_userCredentials;
}

auto UserServices::login(const QString &username, const QByteArray &password) -> QFuture<UserCredentials>
{
    return QtConcurrent::run
    (
        [this, username = std::move(username), password = std::move(password)] () -> UserCredentials
        {
            QSharedPointer<QNetworkAccessManager> manager = CoreServices::networkAccessManager().toStrongRef();

            if(!manager)
            {
                Q_EMIT loginFailed();
                return {};
            }

            QNetworkRequest request
            (
                QUrl
                (
                    QStringLiteral("%1/%2/%3").arg
                    (
                        KOMPLEX_API_HOST,
                        KOMPLEX_API_VERSION,
                        KOMPLEX_ENDPOINT_USER_AUTH
                    )
                )
            );

            QJsonObject loginRootObject
            {
                {
                    QString("username"),
                    QJsonValue::fromVariant(username.toUtf8())
                },
                {
                    QString("password"),
                    QJsonValue::fromVariant(password.toBase64())
                }
            };

            QJsonDocument loginDocument(loginRootObject);

            QNetworkReply *reply = manager->post(request, loginDocument.toJson(QJsonDocument::Compact));

            while(!reply->isFinished())
            {
                std::chrono::milliseconds(10);
            }

            if(reply->error() != QNetworkReply::NoError)
            {
                setErrorString(reply->errorString());
                throw std::exception();
            }

            QByteArray data = reply->readAll();

            QJsonParseError parseError;
            QJsonDocument document = QJsonDocument::fromJson(data, &parseError);

            if(parseError.error != QJsonParseError::NoError)
            {
                setErrorString(parseError.errorString());
            }

            QJsonObject rootObject;

            if(!rootObject.value("success").toBool())
            {
                QJsonObject dataObject = rootObject.value(QStringLiteral("data")).toObject();

                setErrorString
                (
                    QStringLiteral("Login failure (%1): %2").arg
                    (
                        dataObject.value(QStringLiteral("errorCode")).toString(),
                        dataObject.value(QStringLiteral("message")).toString()
                    )
                );
            }

            UserCredentials credentials
            {
                .username = username.toUtf8(),
                .sessionToken = rootObject.value
                (
                    QStringLiteral("jwt")
                ).toString().toUtf8(),
                .expiry = QDateTime::currentDateTime()
            };

            setUserCredentials(credentials);

            Q_EMIT loginComplete();
            return credentials;
        }
    );
}