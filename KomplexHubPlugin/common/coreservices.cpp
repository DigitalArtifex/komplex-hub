#include "coreservices.h"
#include "logging.h"

QNetworkAccessManager *CoreServices::m_networkAccessManager = new QNetworkAccessManager();

auto CoreServices::networkAccessManager() -> QNetworkAccessManager *
{
    return m_networkAccessManager;
}

auto CoreServices::sessionToken() -> QByteArray
{
    LOG_ERROR_X(true,
        "CoreServices::sessionToken",
        "User services not yet implements",
        {}
    );
}
