#include "qwallet.h"
#include "../3rdparty/qtkeychain/qtkeychain/keychain.h"

auto QWallet::read(const QString &service, const QString &key) const -> QFuture<QByteArray>
{
    return QtConcurrent::run
    (
        [this, service(service), key(key)]() -> QByteArray
        {
            QEventLoop loop;
            QByteArray value;

            auto readCredentialJob = new QKeychain::ReadPasswordJob(service);
            readCredentialJob->setAutoDelete(true);
            readCredentialJob->setKey(key);

            QObject::connect
            (
                readCredentialJob,
                &QKeychain::ReadPasswordJob::finished,
                this,
                [this, key, &loop, &value](QKeychain::Job *job)
                {
                    auto j = static_cast<QKeychain::ReadPasswordJob*>(job);
                    if (j->error() == QKeychain::NoError)
                    {
                        value = j->binaryData();
                    }
                    else
                    {
                        m_errorString = qPrintable(j->errorString());
                        throw std::exception();
                    }

                    loop.quit();
                }
            );

            readCredentialJob->start();
            loop.exec();

            return value;
        }
    );
}

auto QWallet::write(const QString &service, const QString &key, const QByteArray &value) -> QFuture<void>
{
    return QtConcurrent::run
    (
        [this, service(service), key(key), value(value)]() -> void
        {
            QEventLoop loop;
            bool success;

            auto writeCredentialJob = new QKeychain::WritePasswordJob(service);
            writeCredentialJob->setAutoDelete(true);
            writeCredentialJob->setKey(key);
            writeCredentialJob->setBinaryData(value);

            QObject::connect
            (
                writeCredentialJob,
                &QKeychain::WritePasswordJob::finished,
                this,
                [this, key, &loop, &success](QKeychain::Job *job)
                {
                    auto j = static_cast<QKeychain::ReadPasswordJob*>(job);
                    if (j->error() != QKeychain::NoError)
                    {
                        setErrorString(qPrintable(j->errorString()));
                        throw std::exception();
                    }

                    loop.quit();
                }
            );

            writeCredentialJob->start();
            loop.exec();
        }
    );
}

auto QWallet::remove(const QString &service, const QString &key) -> QFuture<void>
{
    return QtConcurrent::run
    (
        [this, service(service), key(key)]() -> void
        {
            QEventLoop loop;
            bool value;

            auto deleteCredentialJob = new QKeychain::DeletePasswordJob(service);
            deleteCredentialJob->setAutoDelete(true);
            deleteCredentialJob->setKey(key);

            QObject::connect
            (
                deleteCredentialJob,
                &QKeychain::DeletePasswordJob::finished,
                this,
                [this, key, &loop, &value](QKeychain::Job *job)
                {
                    auto j = static_cast<QKeychain::ReadPasswordJob*>(job);
                    if (j->error() != QKeychain::NoError)
                    {
                        m_errorString = qPrintable(j->errorString());
                        throw std::exception();
                    }

                    loop.quit();
                }
            );

            deleteCredentialJob->start();
            loop.exec();
        }
    );
}

QWallet::QWallet(QObject *parent) : QObject(parent)
{

}

QWallet::~QWallet()
{

}

auto QWallet::errorString() const -> const QString &
{
    return m_errorString;
}
