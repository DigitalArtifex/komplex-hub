#include "userservices.h"
#include "common/komplex_global.h"
#include "coreservices.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <exception>
#include <qcontainerfwd.h>
#include <qstandardpaths.h>
#include <qstringview.h>

UserServices::UserServices(QObject *parent) : QObject(parent)
{
    m_wallet = new QWallet(this);
}

UserServices::~UserServices()
{
    if(m_wallet)
    {
        m_wallet->deleteLater();
    }
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
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

auto UserServices::savePassword(const QString &username, const QByteArray &password) -> void
{
    QFuture saveAttempt = m_wallet->write
    (
        KOMPLEX_KEYCHAIN_SERVICE_NAME,
        username,
        password
    );
}

auto UserServices::readPassword(const QString &username) -> QFuture<QByteArray>
{
    return m_wallet->read(KOMPLEX_KEYCHAIN_SERVICE_NAME, username);
}

auto UserServices::readToken(const QString &username) -> QFuture<UserCredentials>
{
    return QtConcurrent::run
    (
        [this, username] () -> UserCredentials
        {
            UserCredentials credentials;

            QFuture readAttempt = m_wallet->read
            (
                KOMPLEX_KEYCHAIN_SERVICE_NAME, 
                username + QStringLiteral("_token")
            );

            QString _username = username;

            readAttempt
                .then
                (
                    [this, &credentials, _username] (const QByteArray &value) -> void
                    {
                        auto parts = value.split(PART_SEPARATOR);
                        QDateTime expiry;

                        if(parts.count() == 1)
                        {
                            setErrorString(QStringLiteral("Token malformed"));
                            throw std::exception();
                        }

                        if(parts[0].length() != sizeof(qint64))
                        {
                            setErrorString(QStringLiteral("Token expiry malformed"));
                            throw std::exception();
                        }

                        QByteArray numericData = parts[0];
                        qint64 numericConversion = 0;

                        for(int i = 0; i < sizeof(qint64); ++i)
                        {
                            numericConversion |= 
                            (
                                static_cast<qint64>
                                (
                                    (
                                        static_cast<qint8>(numericData[i]) &
                                        0xFF
                                    )
                                    << (i * sizeof(qint8))
                                )
                            );
                        }

                        expiry = QDateTime::fromMSecsSinceEpoch(numericConversion);

                        if(parts[1].isEmpty() && parts.count() == 3)
                        {
                            parts[1] = parts[2];
                        }

                        credentials = UserCredentials
                        {
                            .username = _username.toUtf8(),
                            .sessionToken = parts[1],
                            .expiry = expiry
                        };
                    }
                )
                .onFailed
                (
                    [this] () -> void
                    {
                        setErrorString(m_wallet->errorString());
                        throw std::exception();
                    }
                );

            readAttempt.waitForFinished();

            return credentials;
        }
    );
}

auto UserServices::saveToken(const QString &username, const QByteArray &token, const QDateTime &expiry) -> void
{
    QByteArray data;
    qint64 numericData = expiry.toMSecsSinceEpoch();

    for(int i = 0; i < sizeof(qint64); ++i)
    {
        data[i] = 0xFF & static_cast<qint8>(numericData >> (i * sizeof(qint8)));
    }

    data += PART_SEPARATOR + token;

    QFuture saveAttempt = m_wallet->write
    (
        KOMPLEX_KEYCHAIN_SERVICE_NAME,
        username + QString("_token"),
        data
    );
}

auto UserServices::instance() -> QWeakPointer<UserServices>
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
        [this, username, password] () -> UserCredentials
        {
            QSharedPointer<QNetworkAccessManager> manager = CoreServices::networkAccessManager().toStrongRef();

            if(!manager)
            {
                Q_EMIT loginFailed();
                setErrorString(QStringLiteral("Network manager reference deleted"));
                throw std::exception();
            }

            saveDefaultUsername(username);

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
                throw std::exception();
            }

            QJsonObject rootObject = document.object();
            QJsonObject dataObject = rootObject.value(QStringLiteral("data")).toObject();

            if(!rootObject.value("success").toBool())
            {
                setErrorString
                (
                    QStringLiteral("Login failure (%1): %2").arg
                    (
                        dataObject.value(QStringLiteral("errorCode")).toString(),
                        dataObject.value(QStringLiteral("message")).toString()
                    )
                );
                throw std::exception();
            }

            QString expiryData = dataObject.value(QStringLiteral("expires")).toString();
            bool converted = false;
            qint64 secsSinceEpoch = expiryData.toLongLong(&converted);

            if(!converted)
            {
                setErrorString(QStringLiteral("Malformed expiry data"));
                throw std::exception();
            }

            UserCredentials credentials
            {
                .username = username.toUtf8(),
                .sessionToken = dataObject.value
                (
                    QStringLiteral("jwt")
                ).toString().toUtf8(),
                .expiry = QDateTime::fromSecsSinceEpoch(secsSinceEpoch)
            };

            savePassword(username, password);
            saveToken(username, credentials.sessionToken, credentials.expiry);
            setUserCredentials(credentials);

            Q_EMIT loginComplete();
            return credentials;
        }
    );
}

auto UserServices::saveDefaultUsername(const QString &username) -> void
{
    QDir configDirectory(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));

    if(!configDirectory.exists())
    {
        if(!configDirectory.mkpath(configDirectory.absolutePath()))
        {
            setErrorString(QStringLiteral("Could not make config directory"));
            throw std::exception();
        }
    }

    QFile usernameFile(configDirectory.absoluteFilePath(QStringLiteral("uname")));

    if(!usernameFile.open(QFile::ReadWrite | QFile::Truncate))
    {
        setErrorString(QStringLiteral("Could not open uname file"));
        throw std::exception();
    }

    QByteArray data = username.toUtf8().toBase64();

    if(usernameFile.write(data) != data.length())
    {
        setErrorString(QStringLiteral("Could not write uname file"));
    }

    usernameFile.close();
}

auto UserServices::defaultUsername() -> QFuture<QString>
{
    return QtConcurrent::run
    (
        [this] () -> QString
        {        
            QDir configDirectory(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));

            if(!configDirectory.exists())
            {
                if(!configDirectory.mkpath(configDirectory.absolutePath()))
                {
                    setErrorString(QStringLiteral("Could not make config directory"));
                    throw std::exception();
                }
            }

            QFile usernameFile(configDirectory.absoluteFilePath(QStringLiteral("uname")));

            if(!usernameFile.open(QFile::ReadOnly))
            {
                setErrorString(QStringLiteral("Could not open uname file"));
                throw std::exception();
            }

            QByteArray data = QByteArray::fromBase64(usernameFile.readAll());

            usernameFile.close();

            return QString(data);
        }
    );
}