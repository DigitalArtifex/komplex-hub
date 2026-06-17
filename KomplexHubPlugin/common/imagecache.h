#ifndef IMAGECACHE_H
#define IMAGECACHE_H

#include "komplex_global.h"
#include <QObject>
#include <QMap>
#include <QString>

struct KOMPLEX_EXPORT ImageCache
{
    QString altText;
    QByteArray averageColor;
    qsizetype height;
    qsizetype width;
    qint64 id;
    bool liked; //probably not needed
    QString photographer;
    QString photographerUrl;
    qint64 photographerId;
    QMap<QString, QString> sources;
    QString url;

    auto operator == (const ImageCache &other) const -> bool
    {
        return (
            other.altText == altText &&
            other.averageColor == averageColor &&
            other.height == height &&
            other.width == width &&
            other.id == id &&
            other.liked == liked &&
            other.photographer == photographer &&
            other.photographerUrl == photographerUrl &&
            other.photographerId == photographerId &&
            other.sources == sources &&
            other.url == url
        );
    }

    auto operator != (const ImageCache &other) const -> bool
    {
        return !(other == *this);
    }
};
Q_DECLARE_METATYPE(ImageCache)


#endif // IMAGECACHE_H
