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
#ifndef SHADERTOYMETADATA_H
#define SHADERTOYMETADATA_H
#include <QObject>
#include <QDateTime>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QUuid>
#include <QProcess>
#include <QTimer>
#include <QMutex>
#include <QDir>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <qqmlintegration.h>

#include "komplex_global.h"

struct KOMPLEX_EXPORT ShaderToyMetadata
{
    QDateTime date;
    QString description;
    quint64 flags = 0;
    bool hasLiked = false;
    QString id;
    quint64 likes = 0;
    QString name;
    quint64 published = 0;
    QStringList tags;
    bool usePreview = false;
    QString username;
    QString version;
    quint64 views = 0;
};

struct KOMPLEX_EXPORT ShaderToyRenderInput
{
    quint8 channel = 0;
    QString ctype;
    QString filter;
    quint64 id = 0;
    QString internal;
    bool published = false;
    QString source;
    bool srgb = false;
    bool verticalFlip = false;
    QString wrap;
};

struct KOMPLEX_EXPORT ShaderToyRenderOutput
{
    const quint8 channel = 0;
    const quint64 id = 0;
};

struct KOMPLEX_EXPORT ShaderToyRenderPass
{
    QByteArray code;
    QString description;
    QList<ShaderToyRenderInput> inputs;
    QString name;
    QList<ShaderToyRenderOutput> outputs;
    QString type;
};

enum KOMPLEX_EXPORT ShaderToyErrorCode
{
    NoError = 0,
    NetworkError = (0x10 << 24),
    MediaError = (0x20 << 24),
    DiskError = (0x30 << 24),
    CompileError = (0x40 << 24)
};

struct KOMPLEX_EXPORT ShaderToyError
{
    QString message;
    ShaderToyErrorCode code;
};

struct KOMPLEX_EXPORT ShaderToyEntry
{
    enum Status
    {
        Idle,
        Loading,
        Compiling,
        Compiled,
        Error
    };

    ShaderToyMetadata metadata;
    QList<ShaderToyRenderPass> renderPasses;
    QByteArray data;
    ShaderToyError error;
    QStringList videoSelections;
    Status status = Idle;
};

Q_DECLARE_METATYPE(ShaderToyEntry)
Q_DECLARE_METATYPE(ShaderToyError)
#endif // SHADERTOYMETADATA_H
