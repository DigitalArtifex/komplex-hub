// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlNetworkAccessManagerFactory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <qobject.h>
#include <qqml.h>
#include <QPalette>
#include <QStyle>
#include <QQuickStyle>
#include <QTextStream>
#include <QSurface>
#include <QStyleFactory>
#include <QColorSpace>
#include <QQuickWindow>
#include <QSurfaceFormat>
#include <QSGRendererInterface>
#include <QScreen>
#include "autogen/environment.h"
#include "KomplexHubPlugin/common/komplex_global.h"

#ifdef KOMPLEX_LOCAL_DEV
class DebugNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager *create(QObject *parent) override;
};

QNetworkAccessManager *DebugNetworkAccessManagerFactory::create(QObject *parent)
{
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);

    QObject::connect
    (
        nam,
        &QNetworkAccessManager::sslErrors,
        nam,
        [] (QNetworkReply *reply, const QList<QSslError> &errors)
        {
            reply->ignoreSslErrors();
        }
    );
    return nam;
}
#endif

int main(int argc, char *argv[])
{
    set_qt_environment();
    QCoreApplication::setOrganizationName("DigitalArtifex");
    QCoreApplication::setApplicationName("KomplexHub");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(mainQmlFile);
    QObject::connect(
                &engine, &QQmlApplicationEngine::objectCreated, &app,
                [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

#ifdef KOMPLEX_LOCAL_DEV
    engine.setNetworkAccessManagerFactory(new DebugNetworkAccessManagerFactory);
#endif

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}