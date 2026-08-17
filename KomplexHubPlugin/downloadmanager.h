/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex
 *  https://digitalartifex.dev - https://github.com/DigitalArtifex
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>
 */
#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>
#include <QUuid>
#include <QQmlEngine>
#include <QProcess>
#include <QDir>
#include <QStandardPaths>
#include <QUuid>
#include <QtConcurrent/QtConcurrentRun>

#include "common/komplex_global.h"
#include "common/shadertoymetadata.h"
#include "common/shaderpack.h"
#include "packcompiler.h"

class KOMPLEX_EXPORT DownloadManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:

    enum State
    {
        Idle,
        Downloading,
        Compiling,
        Installing,
        Complete,
        Error
    };
    Q_ENUM(State)

    enum RequestType
    {
        Get,
        Post
    };

    explicit DownloadManager(QObject *parent = nullptr);

    /**
     * @brief downloadImage
     * Downloads the image to tmp from the given url and generates a pack
     * out of it
     * @param url
     */
    Q_INVOKABLE auto downloadImage
    (
        const QString &author,
        const QString &authorId,
        const QString &description,
        const QUrl &url
    ) -> void;

    /**
     * @brief downloadVideo
     * Downloads the video to tmp from the given url and generates a pack
     * out of it
     * @param author
     * @param authorId
     * @param description
     * @param url
     */
    Q_INVOKABLE auto downloadVideo
    (
        const QString &author,
        const QString &authorId,
        const QString &description,
        const QUrl &url
    ) -> void;

    /**
     * @brief downloadPack
     * Attempts to download, compile and install the wallpaper
     * pack from the komplex endpoint.
     * @param id
     * @return File info of the installed pack
     */
    Q_INVOKABLE auto downloadPack(const QString &id) -> void;

    auto compilerOutput() const -> const QString & { return m_compilerOutput; }
    auto errorTitle() const -> const QString & { return m_errorTitle; }
    auto errorMessage() const -> const QString & { return m_errorMessage; }
    auto state() const -> State { return m_state; }
    Q_INVOKABLE auto reset() -> void;
    auto downloadProgress() -> qreal { return m_downloadProgress; }
    auto compileProgress() -> qreal;
    auto compileSteps() -> qint64;
    auto compileStepsCompleted() -> qint64;

    auto downloadSize() const -> qint64 { return m_downloadSize; }
    auto setDownloadSize(qint64 downloadSize) -> void;

    auto downloadedBytes() const -> qint64 { return m_downloadedBytes; }
    auto setDownloadedBytes(qint64 downloadedBytes) -> void;

    auto lastInstalledFile() const -> const QString &;
    auto setLastInstalledFile(const QString &lastInstalledFile) -> void;

protected:
    auto setError(const QString &title, const QString &message) -> void;
    auto setCompilerOutput(const QString &compilerOutput) -> void;
    auto setState(State state) -> void;
    auto setDownloadProgress(qreal progress) -> void;

signals:
    auto compilerOutputChanged() -> void;
    auto errorChanged() -> void;
    auto stateChanged() -> void;
    auto downloadProgressChanged() -> void;
    auto compileProgressChanged() -> void;
    auto downloadComplete(const QString &uri) -> void;
    auto downloadSizeChanged() -> void;
    auto downloadedBytesChanged() -> void;
    auto compileStepsChanged() -> void;
    auto compileStepsCompletedChanged() -> void;

    auto lastInstalledFileChanged() -> void;

private:
    auto compile(const QUrl &uri) noexcept(false) -> QUrl;
    auto install(const QUrl &uri) noexcept(false) -> QUrl;
    auto download(const QNetworkRequest &request, const QString &id, RequestType type = Get) -> QFuture<QUrl>;
    auto readShaderToyEntry(const QUrl &uri) noexcept(false) -> ShaderToyEntry;

    QString m_lastInstalledFile;

    QString m_compilerOutput;
    QString m_errorTitle;
    QString m_errorMessage;

    qreal m_downloadProgress = 0;
    qint64 m_downloadSize = 0;
    qint64 m_downloadedBytes = 0;
    qreal m_compileProgress = 0;
    State m_state;

    QMutex m_downloadMutex;

    PackCompiler *m_compiler = nullptr;

    QProcess m_moveProcess;

    QFuture<QUrl> m_downloadFuture;

    Q_PROPERTY(QString compilerOutput READ compilerOutput WRITE setCompilerOutput NOTIFY compilerOutputChanged FINAL)
    Q_PROPERTY(QString errorTitle READ errorTitle NOTIFY errorChanged FINAL)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged FINAL)
    Q_PROPERTY(qreal downloadProgress READ downloadProgress NOTIFY downloadProgressChanged FINAL)
    Q_PROPERTY(qreal compileProgress READ compileProgress NOTIFY compileProgressChanged FINAL)
    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
    Q_PROPERTY(qint64 downloadSize READ downloadSize WRITE setDownloadSize NOTIFY downloadSizeChanged FINAL)
    Q_PROPERTY(qint64 downloadedBytes READ downloadedBytes WRITE setDownloadedBytes NOTIFY downloadedBytesChanged FINAL)
    Q_PROPERTY(qint64 compileSteps READ compileSteps NOTIFY compileStepsChanged FINAL)
    Q_PROPERTY(qint64 compileStepsCompleted READ compileStepsCompleted NOTIFY compileStepsCompletedChanged FINAL)
    Q_PROPERTY(QString lastInstalledFile READ lastInstalledFile WRITE setLastInstalledFile NOTIFY lastInstalledFileChanged FINAL)
};
Q_DECLARE_METATYPE(DownloadManager)

#endif // DOWNLOADMANAGER_H
