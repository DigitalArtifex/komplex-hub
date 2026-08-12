#ifndef SHADERPACK_H
#define SHADERPACK_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QDir>
#include <QMap>
#include <QProcess>
#include <QEventLoop>
#include <QtQml/qqmlregistration.h>

#include "komplex_global.h"

class KOMPLEX_EXPORT ShaderChannel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Type
    {
        Image,
        Video,
        Shader,
        Cubemap,
        Audio,
        Scene,
        Model
    };
    Q_ENUM(Type)

    enum MirrorMode
    {
        MirrorVertical,
        MirrorHorizontal,
        NoMirror
    };
    Q_ENUM(MirrorMode)

    enum Format
    {
        RGB8A,
        RGB16F,
        RGB32F
    };
    Q_ENUM(Format)

    enum TextureRepeat
    {
        ClampToEdge,
        RepeatVertically,
        RepeatHorizontally,
        Repeat
    };
    Q_ENUM(TextureRepeat)

    inline auto source() const -> QString { return m_source; }
    inline auto setSource(const QString &source) -> void
    {
        if(source == this->source())
        {
            return;
        }

        m_source = source;
        Q_EMIT sourceChanged();
    }

    inline auto type() const -> Type { return m_type; }
    inline auto setType(Type type) -> void
    {
        if(type == this->type())
        {
            return;
        }

        m_type = type;
        Q_EMIT typeChanged();
    }

    inline auto mirrorMode() const -> MirrorMode { return m_mirrorMode; }
    inline auto setMirrorMode(MirrorMode mode)
    {
        if(mirrorMode() == mode)
        {
            return;
        }

        m_mirrorMode = mode;
        Q_EMIT mirrorModeChanged();
    }

    inline auto textureRepeat() const -> TextureRepeat { return m_textureRepeat; }
    inline auto setTextureRepeat(TextureRepeat mode) -> void
    {
        if(textureRepeat() == mode)
        {
            return;
        }

        m_textureRepeat = mode;
        Q_EMIT textureRepeatChanged();
    }

    inline auto resolutionY() const -> qsizetype { return m_resolutionY; }
    inline auto setResolutionY(qsizetype resolution)
    {
        if(resolutionY() == resolution)
        {
            return;
        }

        m_resolutionY = resolution;
        Q_EMIT resolutionYChanged();
    }

    inline auto resolutionX() const -> qsizetype { return m_resolutionX; }
    inline auto setResolutionX(qsizetype resolution)
    {
        if(resolutionX() == resolution)
        {
            return;
        }

        m_resolutionX = resolution;
        Q_EMIT resolutionXChanged();
    }

    inline auto resolutionScale() const -> qreal { return m_resolutionScale; }
    inline auto setResolutionScale(qreal scale)
    {
        if(qFuzzyCompare(resolutionScale(), scale))
        {
            return;
        }

        m_resolutionScale = scale;
        Q_EMIT resolutionScaledChanged();
    }

    inline auto speed() const -> qreal { return m_speed; }
    inline auto setSpeed(qreal speed)
    {
        if(qFuzzyCompare(this->speed(), speed))
        {
            return;
        }

        m_speed = speed;
        Q_EMIT speedChanged();
    }

    inline auto mouseSpeedBias() const -> qreal { return m_mouseSpeedBias; }
    inline auto setMouseSpeedBias(qreal bias)
    {
        if(qFuzzyCompare(mouseSpeedBias(), bias))
        {
            return;
        }

        m_mouseSpeedBias = bias;
        Q_EMIT mouseSpeedBiasChanged();
    }

    inline auto bufferA() const -> ShaderChannel* { return m_bufferA; }
    inline auto setBufferA(ShaderChannel *buffer) -> void
    {
        if (m_bufferA == buffer)
        {
            return;
        }

        m_bufferA = buffer;
        Q_EMIT bufferAChanged();
    }

    inline auto bufferB() const -> ShaderChannel* { return m_bufferB; }
    inline auto setBufferB(ShaderChannel *buffer) -> void
    {
        if (m_bufferB == buffer)
        {
            return;
        }

        m_bufferB = buffer;
        Q_EMIT bufferBChanged();
    }

    inline auto bufferC() const -> ShaderChannel* { return m_bufferC; }
    inline auto setBufferC(ShaderChannel *buffer) -> void
    {
        if (m_bufferC == buffer)
        {
            return;
        }

        m_bufferC = buffer;
        Q_EMIT bufferCChanged();
    }

    inline auto bufferD() const -> ShaderChannel* { return m_bufferD; }
    inline auto setBufferD(ShaderChannel *buffer) -> void
    {
        if (m_bufferD == buffer)
        {
            return;
        }

        m_bufferD = buffer;
        Q_EMIT bufferDChanged();
    }

    inline auto channel0() const -> ShaderChannel* { return m_channel0; }
    inline auto setChannel0(ShaderChannel *channel) -> void
    {
        if (m_channel0 == channel)
        {
            return;
        }

        m_channel0 = channel;
        Q_EMIT channel0Changed();
    }

    inline auto channel1() const -> ShaderChannel* { return m_channel1; }
    inline auto setChannel1(ShaderChannel *channel) -> void
    {
        if (m_channel1 == channel)
        {
            return;
        }

        m_channel1 = channel;
        Q_EMIT channel1Changed();
    }

    inline auto channel2() const -> ShaderChannel* { return m_channel2; }
    inline auto setChannel2(ShaderChannel *channel) -> void
    {
        if (m_channel2 == channel)
        {
            return;
        }

        m_channel2 = channel;
        Q_EMIT channel2Changed();
    }

    inline auto channel3() const -> ShaderChannel* { return m_channel3; }
    inline auto setChannel3(ShaderChannel *channel) -> void
    {
        if (m_channel3 == channel)
        {
            return;
        }

        m_channel3 = channel;
        Q_EMIT channel3Changed();
    }

    inline auto frameBufferChannel() const -> qint8 { return m_frameBufferChannel; };
    inline auto setFrameBufferChannel(qint8 frameBufferChannel) -> void
    {
        if (m_frameBufferChannel == frameBufferChannel)
        {
            return;
        }

        m_frameBufferChannel = frameBufferChannel;
        Q_EMIT frameBufferChannelChanged();
    }

    inline auto invert() const -> bool { return m_invert; }
    inline auto setInvert(bool invert) -> void
    {
        if (m_invert == invert)
        {
            return;
        }

        m_invert = invert;
        emit invertChanged();
    }

    inline auto mipmap() const -> bool { return m_mipmap; }
    inline auto setMipmap(bool on)
    {
        if(on == mipmap())
        {
            return;
        }

        m_mipmap = on;
        Q_EMIT mipmapChanged();
    }

    inline auto format() const -> Format { return m_format; }
    inline auto setFormat(Format format)
    {
        if(this->format() == format)
        {
            return;
        }

        m_format = format;
        Q_EMIT formatChanged();
    }

Q_SIGNALS:
    auto sourceChanged() -> void;
    auto typeChanged() -> void;
    auto mirrorModeChanged() -> void;
    auto textureRepeatChanged() -> void;
    auto resolutionXChanged() -> void;
    auto resolutionYChanged() -> void;
    auto resolutionScaledChanged() -> void;
    auto speedChanged() -> void;
    auto mouseSpeedBiasChanged() -> void;
    auto bufferAChanged() -> void;
    auto bufferBChanged() -> void;
    auto bufferCChanged() -> void;
    auto bufferDChanged() -> void;
    auto channel0Changed() -> void;
    auto channel1Changed() -> void;
    auto channel2Changed() -> void;
    auto channel3Changed() -> void;
    auto resolutionScaleChanged() -> void;
    auto frameBufferChannelChanged() -> void;
    auto invertChanged() -> void;
    auto mipmapChanged() -> void;
    auto formatChanged() -> void;

private:
    ShaderChannel *m_bufferA = nullptr;
    ShaderChannel *m_bufferB = nullptr;
    ShaderChannel *m_bufferC = nullptr;
    ShaderChannel *m_bufferD = nullptr;

    ShaderChannel *m_channel0 = nullptr;
    ShaderChannel *m_channel1 = nullptr;
    ShaderChannel *m_channel2 = nullptr;
    ShaderChannel *m_channel3 = nullptr;

    QString m_source;
    Type m_type = Image;
    MirrorMode m_mirrorMode = MirrorVertical;
    TextureRepeat m_textureRepeat = ClampToEdge;
    Format m_format = RGB8A;

    qsizetype m_resolutionX = 0;
    qsizetype m_resolutionY = 0;
    qreal m_resolutionScale = 1.0;
    qreal m_speed = 1.0;
    qreal m_mouseSpeedBias = 1.0;

    qint8 m_frameBufferChannel = -1;
    bool m_invert = false;
    bool m_mipmap = false;

    Q_PROPERTY(ShaderChannel *bufferA READ bufferA WRITE setBufferA NOTIFY bufferAChanged FINAL)
    Q_PROPERTY(ShaderChannel *bufferB READ bufferB WRITE setBufferB NOTIFY bufferBChanged FINAL)
    Q_PROPERTY(ShaderChannel *bufferC READ bufferC WRITE setBufferC NOTIFY bufferCChanged FINAL)
    Q_PROPERTY(ShaderChannel *bufferD READ bufferD WRITE setBufferD NOTIFY bufferDChanged FINAL)
    Q_PROPERTY(ShaderChannel *channel0 READ channel0 WRITE setChannel0 NOTIFY channel0Changed FINAL)
    Q_PROPERTY(ShaderChannel *channel1 READ channel1 WRITE setChannel1 NOTIFY channel1Changed FINAL)
    Q_PROPERTY(ShaderChannel *channel2 READ channel2 WRITE setChannel2 NOTIFY channel2Changed FINAL)
    Q_PROPERTY(ShaderChannel *channel3 READ channel3 WRITE setChannel3 NOTIFY channel3Changed FINAL)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged FINAL)
    Q_PROPERTY(MirrorMode mirrorMode READ mirrorMode WRITE setMirrorMode NOTIFY mirrorModeChanged FINAL)
    Q_PROPERTY(TextureRepeat textureRepeat READ textureRepeat WRITE setTextureRepeat NOTIFY textureRepeatChanged FINAL)
    Q_PROPERTY(qsizetype resolutionX READ resolutionX WRITE setResolutionX NOTIFY resolutionXChanged FINAL)
    Q_PROPERTY(qsizetype resolutionY READ resolutionY WRITE setResolutionY NOTIFY resolutionYChanged FINAL)
    Q_PROPERTY(qreal resolutionScale READ resolutionScale WRITE setResolutionScale NOTIFY resolutionScaleChanged FINAL)
    Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged FINAL)
    Q_PROPERTY(qreal mouseSpeedBias READ mouseSpeedBias WRITE setMouseSpeedBias NOTIFY mouseSpeedBiasChanged FINAL)
    Q_PROPERTY(qint8 frameBufferChannel READ frameBufferChannel WRITE setFrameBufferChannel NOTIFY frameBufferChannelChanged FINAL)
    Q_PROPERTY(bool invert READ invert WRITE setInvert NOTIFY invertChanged FINAL)
    Q_PROPERTY(bool mipmap READ mipmap WRITE setMipmap NOTIFY mipmapChanged FINAL)
    Q_PROPERTY(Format format READ format WRITE setFormat NOTIFY formatChanged FINAL)
};

Q_DECLARE_METATYPE(ShaderChannel)

class KOMPLEX_EXPORT ShaderPack : public ShaderChannel
{
    Q_OBJECT
    QML_ELEMENT

    QString m_author;
    QString m_description;
    QString m_engine;
    QString m_file;
    QString m_id;
    QString m_license;
    QString m_name;
    QString m_version;

    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString engine READ engine WRITE setEngine NOTIFY engineChanged)
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString license READ license WRITE setLicense NOTIFY licenseChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)

Q_SIGNALS:
    void authorChanged();
    void descriptionChanged();
    void engineChanged();
    void idChanged();
    void licenseChanged();
    void nameChanged();
    void versionChanged();
    void fileChanged();

public:

    QString author() const { return m_author; }
    void setAuthor(const QString& author)
    {
        if(author != m_author)
        {
            m_author = author;
            Q_EMIT authorChanged();
        }
    }

    QString description() const { return m_description; }
    void setDescription(const QString& description)
    {
        if(description != m_description)
        {
            m_description = description;
            Q_EMIT descriptionChanged();
        }
    }

    QString engine() const { return m_engine; }
    void setEngine(const QString& engine)
    {
        if(engine != m_engine)
        {
            m_engine = engine;
            Q_EMIT engineChanged();
        }
    }

    QString file() const { return m_file; }
    void setFile(const QString& file)
    {
        if(file != m_file)
        {
            m_file = file;
            Q_EMIT fileChanged();
        }
    }

    QString id() const { return m_id; }
    void setId(const QString& id)
    {
        if(id != m_id)
        {
            m_id = id;
            Q_EMIT idChanged();
        }
    }

    QString license() const { return m_license; }
    void setLicense(const QString& license)
    {
        if(license != m_license)
        {
            m_license = license;
            Q_EMIT licenseChanged();
        }
    }

    QString name() const { return m_name; }
    void setName(const QString& name)
    {
        if(name != m_name)
        {
            m_name = name;
            Q_EMIT nameChanged();
        }
    }

    QString version() const { return m_version; }
    void setVersion(const QString& version)
    {
        if(version != m_version)
        {
            m_version = version;
            Q_EMIT versionChanged();
        }
    }

    auto json() -> QJsonDocument
    {
        QJsonObject rootObject;

        rootObject[QStringLiteral("author")] = author();
        rootObject[QStringLiteral("version")] = version();
        rootObject[QStringLiteral("name")] = name();
        rootObject[QStringLiteral("id")] = id();
        rootObject[QStringLiteral("engine")] = engine();
        rootObject[QStringLiteral("license")] = license();

        QJsonObject bufferObject = bufferJson(this);
        QJsonObject channelObject = channelJson(this);

        const QStringList bufferKeys = bufferObject.keys();
        for(const QString &key : bufferKeys)
        {
            rootObject[key] = bufferObject[key];
        }

        const QStringList channelKeys = channelObject.keys();
        for(const QString &key : channelKeys)
        {
            rootObject[key] = channelObject[key];
        }

        return QJsonDocument
        {
            rootObject
        };
    }

private:

    auto bufferJson(ShaderChannel *channel) -> QJsonObject
    {
        QJsonObject channelObject;

        if(channel->bufferA())
        {
            channelObject[QStringLiteral("bufferA")] = channelJson(channel->bufferA());
        }
        if(channel->bufferB())
        {
            channelObject[QStringLiteral("bufferB")] = channelJson(channel->bufferB());
        }
        if(channel->bufferC())
        {
            channelObject[QStringLiteral("bufferC")] = channelJson(channel->bufferC());
        }
        if(channel->bufferD())
        {
            channelObject[QStringLiteral("bufferD")] = channelJson(channel->bufferD());
        }

        return channelObject;
    }

    auto channelJson(ShaderChannel *channel) -> QJsonObject
    {
        if(channel == nullptr)
        {
            return {};
        }

        QJsonObject channelObject;

        if(channel->channel0() != nullptr)
        {
            channelObject[QStringLiteral("channel0")] = channelJson(channel->channel0());
        }

        if(channel->channel1() != nullptr)
        {
            channelObject[QStringLiteral("channel1")] = channelJson(channel->channel1());
        }

        if(channel->channel2() != nullptr)
        {
            channelObject[QStringLiteral("channel2")] = channelJson(channel->channel2());
        }

        if(channel->channel3() != nullptr)
        {
            channelObject[QStringLiteral("channel3")] = channelJson(channel->channel3());
        }

        channelObject[QStringLiteral("source")] = channel->source();
        channelObject[QStringLiteral("type")] = channel->type();
        channelObject[QStringLiteral("texture_mirroring")] = channel->mirrorMode();
        channelObject[QStringLiteral("wrap_mode")] = channel->textureRepeat();
        channelObject[QStringLiteral("invert")] = channel->invert();
        channelObject[QStringLiteral("time_scale")] = channel->speed();
        channelObject[QStringLiteral("resolution_scale")] = channel->resolutionScale();
        channelObject[QStringLiteral("mouse_scale")] = channel->mouseSpeedBias();
        channelObject[QStringLiteral("resolution_x")] = channel->resolutionX();
        channelObject[QStringLiteral("resolution_y")] = channel->resolutionY();
        channelObject[QStringLiteral("frame_buffer_channel")] = channel->frameBufferChannel();
        channelObject[QStringLiteral("format")] = channel->format();
        channelObject[QStringLiteral("mipmap")] = channel->mipmap();

        return channelObject;
    }
};

Q_DECLARE_METATYPE(ShaderPack)
#endif