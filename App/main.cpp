// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QQmlApplicationEngine>
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
#include "autogen/environment.h"

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

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}