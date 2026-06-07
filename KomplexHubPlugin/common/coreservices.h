#ifndef CORESERVICES_H
#define CORESERVICES_H

#include <QObject>
#include <QNetworkAccessManager>

class CoreServices
{
public:

    /**
     * @brief networkAccessManager
     * Plugin-wide QNetworkAccessManager
     * @return
     */
    static auto networkAccessManager() -> QNetworkAccessManager*;

    /**
     * @brief getSessionToken
     * Plugin-wide access to current user session token
     * @return
     */
    static auto sessionToken() -> QByteArray;

private:
    static QNetworkAccessManager *m_networkAccessManager;
};
#endif // CORESERVICES_H
