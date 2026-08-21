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
#ifndef PACKCOMPILER_H
#define PACKCOMPILER_H

#include <QDir>
#include <QFile>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QStandardPaths>
#include <QProcess>
#include <QRegularExpression>
#include <QMutex>
#include <QMutexLocker>
#include <QImage>
#include <QRgb>
#include <QColor>
#include <QtQuick/QQuickItem>
#include <qdir.h>
#include <qobject.h>
#include <qtmetamacros.h>
#include <QPainter>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qcontainerfwd.h>
#include <qmargins.h>
#include <qstandardpaths.h>
#include <qtmetamacros.h>
#include <qurl.h>
#include "common/komplex_global.h"
/**
 * @brief The PackCompiler class
 * [1] Extract Pack (GZip)
 * [2] Verify directory is a pack and has shaders
 * [3] Load shader code
 * [4] Append Common and Global files, if they exist
 * [5] Add uniform and replace variable use
 * [6] Run C Preprocessor (cpp -P) to process macros
 * [7] Append version macro
 * [8] Compile with Qt Shader Baker
 */
class KOMPLEX_EXPORT PackCompiler : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:

    enum State
    {
        Idle,
        Compiling,
        Complete,
        Error
    };
    Q_ENUM(State)

    explicit PackCompiler(QObject *parent = nullptr);
    ~PackCompiler() override;

    auto compilerOutput() const -> const QString & { return m_compilerOutput; }
    auto progress() -> qreal { return m_progress; }
    auto currentStep() -> qint64 { return m_currentStep; }
    auto totalSteps() -> qint64 { return m_totalSteps; }
    auto errorTitle() const -> const QString & { return m_errorTitle; }
    auto errorMessage() const -> const QString & { return m_errorMessage; }
    auto reset() -> void;

    auto process(const QUrl &uri) -> QFuture<QUrl>;

    auto status() const -> const QString & { return m_status; }
    auto state() const -> State { return m_state; }

protected:
    /**
     * @brief prepareShaders
     * Scans the pack's shader directory and sends each file to
     * prepareFile()
     * @param uri
     * Directory to prepare
     */
    auto prepareShaders(const QUrl &uri) noexcept(false) -> void;

    /**
     * @brief prepareFile
     * Prepares the individual file by appending the header, footer,
     * common files and replacing standard variable names
     * @param uri
     */
    auto prepareFile(const QUrl &uri) noexcept(false) -> void;

    /**
     * @brief validateDirectory
     *
     * @param uri
     * @return true
     * @return false
     */
    auto validateDirectory(const QUrl &uri) -> bool;

    auto createDirectory(const QUrl &uri) -> void;

    auto validateUri(const QUrl &uri) noexcept(false) -> void;
    auto extract(const QUrl &sourceUri) noexcept(false) -> QUrl;
    auto copyFile(const QUrl &sourceUri, const QUrl &destinationUri) noexcept(false) -> void;
    auto compile(const QUrl &uri) noexcept(false) -> void;
    auto preprocess(const QUrl &uri) noexcept(false) -> void;
    auto appendVersion(const QUrl &uri) noexcept(false) -> void;
    auto compileShader(const QUrl &uri) noexcept(false) -> void;
    auto loadCommonFragmentData(const QUrl &uri) -> QByteArray;
    auto loadCommonVertexData(const QUrl &uri) -> QByteArray;
    auto loadGlobalData(const QUrl &uri) -> QByteArray;

    auto setError(const QString &title, const QString &message, const QUrl &uri = {}) -> void;
    auto setCompilerOutput(const QString &compilerOutput) -> void;
    auto setState(State state) -> void;
    auto setProgress(qreal progress) -> void;
    auto setTotalSteps(qint64 steps) -> void;
    auto setCurrentStep(qint64 step) -> void;

    auto setStatus(const QString &status) -> void;

    auto incrementCompileStep() -> void;

    auto removeDirectory(const QUrl &uri) -> bool;

signals:
    auto compilerOutputChanged() -> void;
    auto errorOcurred() -> void;
    auto errorTitleChanged() -> void;
    auto errorMessageChanged() -> void;
    auto stateChanged() -> void;
    auto progressChanged() -> void;
    auto currentStepChanged() -> void;
    auto totalStepsChanged() -> void;

    auto compileComplete(const QUrl &uri) -> void;

    auto statusChanged() -> void;
    auto packReady(const QString &) -> void;
    auto errorStepChanged() -> void;
    auto errorRatingChanged() -> void;

private:
    static inline const QStringList m_updateVariables
        {
            QStringLiteral("iTime"),
            QStringLiteral("iTimeDelta"),
            QStringLiteral("iFrameRate"),
            QStringLiteral("iSampleRate"),
            QStringLiteral("iFrame"),
            QStringLiteral("iDate"),
            QStringLiteral("iMouse"),
            QStringLiteral("iResolution"),
            QStringLiteral("iColorTheme"),
            QStringLiteral("iChannelTime"),
            QStringLiteral("iChannelResolution")
        };

    static inline const QByteArray m_version
        {
            R"(#version 450)"
        };

    static inline const QString m_header
        {
            R"(layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float iTime;
    float iTimeDelta;
    float iFrameRate;
    float iSampleRate;
    int iFrame;
    vec4 iDate;
    vec4 iMouse;
    vec3 iResolution;
    float iChannelTime[4];
    vec3 iChannelResolution[4];
    vec4 iColorTheme[4];
} ubuf;

layout(binding = 1) uniform sampler2D iChannel0;
layout(binding = 2) uniform sampler2D iChannel1;
layout(binding = 3) uniform sampler2D iChannel2;
layout(binding = 4) uniform sampler2D iChannel3;

vec2 fragCoord = vec2(qt_TexCoord0.x, 1.0 - qt_TexCoord0.y) * ubuf.iResolution.xy;)"
        };

    static inline const QString m_footer
        {
            R"(void main() {
    vec4 color = vec4(0.0);
    mainImage(color, fragCoord);
    fragColor = color;
})"
        };

    QString m_compilerOutput;
    QString m_errorMessage;
    QString m_errorTitle;
    QString m_status;

    State m_state;

    qreal m_currentStep = 0;
    qreal m_progress = 0;
    qreal m_totalSteps = 0;

    QMutex m_downloadMutex;

    static inline QList<QRegularExpression> m_variableExpressions;

    static inline const QRegularExpression m_commonFragmentExpression = QRegularExpression
    (
        QString("^common\\.frag$"),
        QRegularExpression::CaseInsensitiveOption
    );

    static inline const QRegularExpression m_commonVertexExpression = QRegularExpression
    (
        QString("^common\\.vert$"),
        QRegularExpression::CaseInsensitiveOption
    );

    static inline const QRegularExpression m_fragmentExpression = QRegularExpression
    (
        QString("^.{1,}\\.frag$"),
        QRegularExpression::CaseInsensitiveOption |
            QRegularExpression::DotMatchesEverythingOption
    );

    static inline const QRegularExpression m_vertexExpression = QRegularExpression
    (
        QString("^.{1,}\\.vert$"),
        QRegularExpression::CaseInsensitiveOption |
            QRegularExpression::DotMatchesEverythingOption
    );

    Q_PROPERTY(QString compilerOutput READ compilerOutput WRITE setCompilerOutput NOTIFY compilerOutputChanged FINAL)
    Q_PROPERTY(QString errorTitle READ errorTitle NOTIFY errorTitleChanged FINAL)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged FINAL)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged FINAL)
    Q_PROPERTY(qint64 totalSteps READ totalSteps NOTIFY totalStepsChanged FINAL)
    Q_PROPERTY(qint64 currentStep READ currentStep NOTIFY currentStepChanged FINAL)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(State state READ state NOTIFY stateChanged FINAL)
};

Q_DECLARE_METATYPE(PackCompiler)
#endif // PackCompiler_H
