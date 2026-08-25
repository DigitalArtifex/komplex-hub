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
#include <QtShaderTools>
#include <QtShaderTools/rhi/qshaderbaker.h>
#include <QtConcurrent/qtconcurrentrun.h>
#include <qcontainerfwd.h>
#include <qmargins.h>
#include <qstandardpaths.h>
#include <qtmetamacros.h>
#include <qurl.h>
#include "common/komplex_global.h"

#if __has_include("vulkan/vulkan_core.h")
#define HAS_VULKAN
#include <vulkan/vulkan_core.h>
#endif

#if __has_include("GL/gl.h")
#define HAS_OPENGL
#include <GL/gl.h>
#endif

struct KOMPLEX_EXPORT BuildContext
{
    QMap<QByteArray, QByteArray> commonData;
    QPromise<QUrl> promise;
};

/**
 * @brief The PackCompiler class
 * [1] Extract Pack (GZip)
 * [2] Verify directory is a pack and has shaders
 * [3] Load Common shader code
 * [4] Walk the shader directory
 * [5] Load shader code
 * [6] Append type specific common code and global code
 * [7] Replace uniform variable use
 * [8] Add uniform header
 * [9] Add uniform footer
 * [10] Run preprocessor to process macros
 * [11] Append version macro
 * [12] Compile with QShaderBaker
 * [13] Save
 */
class PackCompiler : public QObject
{
    Q_OBJECT
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

    auto compilerOutput() const -> const QString & { return m_compilerOutput; }
    auto progress() -> qreal { return m_progress; }
    auto currentStep() -> qint64 { return m_currentStep; }
    auto totalSteps() -> qint64 { return m_totalSteps; }
    auto errorTitle() const -> const QString & { return m_errorTitle; }
    auto errorMessage() const -> const QString & { return m_errorMessage; }
    auto status() const -> const QString & { return m_status; }
    auto state() const -> State { return m_state; }
    auto reset() -> void;

    auto build(const QUrl &uri) -> QUrl;

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
    //[1]
    /**
     * @brief extract
     * Extracts the packfile and deletes the source
     * @param uri of the tar.gz pack
     * @return uri of the extracted directory
     */
    auto extract(const QUrl &uri) noexcept(false) -> const QUrl;

    //[2]
    /**
     * @brief validatePack
     * Validates that the directory is a valid pack and has shaders to compile
     * @param uri of the directory provided by extract
     */
    auto validatePack(const QUrl &uri) const noexcept(false) -> void;

    //[3]
    /**
     * @brief loadCommonData
     * Loads the common and global shader code from the shader directory of the uri
     * @param uri
     */
    auto loadCommonData(const QUrl &uri) -> QMap<QByteArray,QByteArray>;

    //[4-5]
    /**
     * @brief processShaders
     * Walks the shader directory, loads the code and calls 6-13 for each
     * @param uri
     * @param context
     */
    auto processShaders(const QUrl &uri, BuildContext *context) -> void;

    //[6]
    /**
     * @brief appendCommonData
     * Appends the type specific common code to the shader data
     * @param data
     * Pointer to shader data
     * @param suffix
     * Shader file suffix
     * @param context
     * Pointer to build context object
     */
    auto appendCommonData(QByteArray *data, const QByteArray &suffix, BuildContext *context) noexcept(true) -> void;

    //[7]
    /**
     * @brief replaceUniformVariables
     * Replaces standard variables such as iFrame with their uniform buffer name
     * @param data
     * Pointer to the shader data
     */
    auto replaceUniformVariables(QByteArray *data) noexcept(true) -> void;

    //[8]
    /**
     * @brief appendUniformHeader
     * Adds header and uniform buffer to the shader data
     * @param data
     * Pointer to the shader data
     */
    auto appendUniformHeader(QByteArray *data) noexcept(true) -> void;

    //[9]
    /**
     * @brief appendUniformFooter
     * Adds the uniform footer to the shader data
     * @param data
     * Pointer to the shader data
     */
    auto appendUniformFooter(QByteArray *data) noexcept(true) -> void;

    //[10]
    /**
     * @brief preprocess
     * I have had issues with defines in code so passing it to a c/c++
     * preprocessor is required. I am now using clang for this as clang
     * is more modern and can use stdin
     * @param data
     */
    auto preprocess(QByteArray *data) noexcept(false) -> void;

    //[11]
    /**
     * @brief appendVersion
     * The version header confuses the preprocessor, so add it at the end
     * @param data
     */
    auto appendVersion(QByteArray *data) noexcept(true) -> void;

    //[12]
    /**
     * @brief compile
     * Compiles the shader data in memory and returns the shader object.
     * @param data
     * Shader data
     * @param filename
     * Filename of the shader
     * @param stage
     * Type of shader, based on suffix of original file
     * @return
     * Compiled shader object
     */
    auto compile(const QByteArray &data, const QString &filename, const QShader::Stage stage) noexcept(false) -> QShader;

    //[13]
    auto save(const QUrl &uri, const QShader &shader) noexcept(false) -> void;

    //helpers
    auto validateUri(const QUrl &uri) const noexcept(false) -> void;
    auto run(const QString &command, const QStringList &arguments, const QByteArray &data = {}) noexcept(false) -> QByteArray;
    auto readFile(const QUrl &uri) noexcept(false) -> QByteArray;
    auto writeFile(const QUrl &uri, const QByteArray &data) noexcept(false) -> void;
    auto copyFile(const QUrl &sourceUri, const QUrl &destinationUri) noexcept(false) -> void;
    auto createDirectory(const QUrl &uri) noexcept(false) -> void;
    auto removeDirectory(const QUrl &uri) noexcept(false) -> void;
    auto getStageFromSuffix(const QString &suffix) const -> QShader::Stage;

    auto setError(const QString &title, const QString &message, const QUrl &uri = {}) -> void;
    auto setCompilerOutput(const QString &compilerOutput) -> void;
    auto setState(State state) -> void;
    auto setProgress(qreal progress) -> void;
    auto setTotalSteps(qint64 steps) -> void;
    auto setCurrentStep(qint64 step) -> void;
    auto setStatus(const QString &status) -> void;
    auto incrementCompileStep() -> void;

#ifdef HAS_VULKAN
    auto vulkanVersion() const -> qint32;
#endif

#ifdef HAS_OPENGL
    auto openGlVersion() const -> qint32;
#endif

    //members
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

    static inline const QStringList m_shaderNameFilters
    {
        QStringLiteral("*.frag"),
        QStringLiteral("*.vert"),
        QStringLiteral("*.tese"),
        QStringLiteral("*.tesc"),
        QStringLiteral("*.geom"),
        QStringLiteral("*.comp"),
        QStringLiteral("*.glsl")
    };

    static inline const QMap<QString, QShader::Stage> m_shaderStages
    {
        {
            QStringLiteral("frag"),
            QShader::FragmentStage
        },
        {
            QStringLiteral("vert"),
            QShader::VertexStage
        },
        {
            QStringLiteral("tese"),
            QShader::TessellationEvaluationStage
        },
        {
            QStringLiteral("tesc"),
            QShader::TessellationControlStage
        },
        {
            QStringLiteral("geom"),
            QShader::GeometryStage
        },
        {
            QStringLiteral("comp"),
            QShader::ComputeStage
        }
    };

    static inline const QByteArray m_version
    {
        R"(#version 450)"
    };

    static inline const QByteArray m_header
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

    static inline const QByteArray m_footer
    {
        R"(
void main() {
    vec4 color = vec4(0.0);
    mainImage(color, fragCoord);
    fragColor = color;
})"
    };

    QMap<QByteArray, QByteArray> m_commonData;
    QMap<QByteArray, QShaderBaker> m_shaders;

    QString m_compilerOutput;
    QString m_errorMessage;
    QString m_errorTitle;
    QString m_status;

    State m_state;

    qreal m_currentStep = 0;
    qreal m_progress = 0;
    qreal m_totalSteps = 0;

    QMutex m_downloadMutex;

    QFuture<QUrl> m_buildFuture;

    static inline QList<QRegularExpression> m_variableExpressions;

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
