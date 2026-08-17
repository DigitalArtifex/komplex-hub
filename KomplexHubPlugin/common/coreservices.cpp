#include "coreservices.h"
#include "logging.h"
#include "userservices.h"

QNetworkAccessManager *CoreServices::s_networkAccessManager = nullptr;
QMutex CoreServices::s_networkAccessMutex;
QSharedPointer<QNetworkAccessManager> CoreServices::s_networkAccessPointer;

auto CoreServices::networkAccessManager() -> QWeakPointer<QNetworkAccessManager>
{
    QMutexLocker locker(&s_networkAccessMutex);

    if(!s_networkAccessPointer)
    {
        if(!s_networkAccessManager)
        {
            s_networkAccessManager = new QNetworkAccessManager(QCoreApplication::instance());
            s_networkAccessManager->setAutoDeleteReplies(true);

            QObject::connect(
                s_networkAccessManager,
                &QNetworkAccessManager::sslErrors,
                s_networkAccessManager,
                [] (QNetworkReply *reply, const QList<QSslError> &errors)
                {
                    for(const auto &error : errors)
                    {
                        LOG_ERROR(
                            "CoreServices::networkAccessManager",
                            error.errorString().toStdString().c_str()
                        );
                    }

    #ifdef KOMPLEX_LOCAL_DEV
                    reply->ignoreSslErrors();
    #endif
                }
            );
        }

        s_networkAccessPointer = QSharedPointer<QNetworkAccessManager>(
            s_networkAccessManager,
            &QNetworkAccessManager::deleteLater
        );
    }

    return s_networkAccessPointer.toWeakRef();
}

auto CoreServices::sessionToken() -> QByteArray
{
    return UserServices::userCredentials().sessionToken;
}

auto CoreServices::screenSize() -> QSize
{
    QScreen *screen = QGuiApplication::primaryScreen();

    if(screen == nullptr)
    {
        return QSize(0,0);
    }

    return screen->size();
}
