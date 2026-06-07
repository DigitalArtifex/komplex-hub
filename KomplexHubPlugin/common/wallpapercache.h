#ifndef WALLPAPERCACHE_H
#define WALLPAPERCACHE_H

#include "komplex_global.h"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDateTime>

struct KOMPLEX_EXPORT WallpaperInstallData
{
    QString uri;
    QString name;
    QString author;
    QString description;
    QString thumbnail;
    QDateTime installDate;
};
Q_DECLARE_METATYPE(WallpaperInstallData)

struct KOMPLEX_EXPORT WallpaperCache
{
    QString uri;
    QString name;
    QString author;
    QString authorId;
    QString description;
    QString thumbnail;
    QDateTime createdDate;
    qreal price = 0;
    QString currency;
    quint64 downloadCount = 0;
    qint8 type = -1;
};
Q_DECLARE_METATYPE(WallpaperCache)

#endif // WALLPAPERCACHE_H
