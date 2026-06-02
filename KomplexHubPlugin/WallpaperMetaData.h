#ifndef WALLPAPERMETADATA_H
#define WALLPAPERMETADATA_H

#include "Komplex_global.h"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDateTime>

struct KOMPLEX_EXPORT WallpaperMetaData {
    QString uri;
    QString name;
    QString author;
    QString description;
    QString thumbnail;
    QDateTime installDate;
};
Q_DECLARE_METATYPE(WallpaperMetaData)

#endif // WALLPAPERMETADATA_H
