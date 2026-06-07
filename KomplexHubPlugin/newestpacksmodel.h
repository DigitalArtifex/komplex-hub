#ifndef NEWESTPACKSMODEL_H
#define NEWESTPACKSMODEL_H
#include <QObject>
#include <QQmlEngine>
#include <QList>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAbstractListModel>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

#include "paginators/newestpackspaginator.h"
#include "common/wallpapercache.h"

class KOMPLEX_EXPORT NewestPacksModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum State
    {
        Idle,
        Loading,
        Error
    };
    Q_ENUM(State)

    enum DataRole {
        UriRole = Qt::UserRole + 1,
        AuthorRole,
        AuthorIdRole,
        DescriptionRole,
        NameRole,
        ThumbnailRole,
        CreatedDateRole,
        PriceRole,
        CurrencyRole,
        DownloadCountRole,
        TypeRole
    };
    Q_ENUM(DataRole)

    explicit NewestPacksModel(QObject *parent = nullptr);

    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;
    auto data(const QModelIndex &index, int role = Qt::DisplayRole) const -> QVariant override;

    auto index(
        int row,
        int column,
        const QModelIndex &parent = QModelIndex()
    ) const -> QModelIndex override;

    auto columnCount(
        const QModelIndex &parent = QModelIndex()
    ) const -> int override;

    auto parent(const QModelIndex &index) const -> QModelIndex override;

    auto setData(
        const QModelIndex &index,
        const QVariant &value,
        int role = Qt::EditRole
    ) -> bool override;

    auto state() const -> State;
    auto setState(State state) -> void;
    auto resetState() -> void;
    auto roleNames() const -> QHash<int, QByteArray> override;

    auto errorString() const -> QString;
    auto setErrorString(const QString &errorString) -> void;

signals:
    auto installedWallpapersChanged() -> void;

    auto stateChanged() -> void;

    auto errorStringChanged() -> void;

private:
    static inline const QHash<int, QByteArray> m_dataRoles =
    {
        {
            static_cast<int>(UriRole),
            QByteArray("uri")
        },
        {
            static_cast<int>(AuthorRole),
            QByteArray("author")
        },
        {
            static_cast<int>(DescriptionRole),
            QByteArray("description")
        },
        {
            static_cast<int>(NameRole),
            QByteArray("name")
        },
        {
            static_cast<int>(ThumbnailRole),
            QByteArray("thumbnail")
        },
        {
            static_cast<int>(CreatedDateRole),
            QByteArray("createdDate")
        },
        {
            static_cast<int>(PriceRole),
            QByteArray("price")
        },
        {
            static_cast<int>(CurrencyRole),
            QByteArray("currency")
        },
        {
            static_cast<int>(DownloadCountRole),
            QByteArray("downloadCount")
        },
        {
            static_cast<int>(TypeRole),
            QByteArray("type")
        }
    };

    static inline const QString m_endpointUri = QString("");
    State m_state = Idle;

    QString m_errorString = QString();

    QString m_installDirectoryUri;

    qint64 m_page = 0;
    qint64 m_resultsPerPage = 0;

    NewestPacksPaginator m_wallpaperCache;

    Q_PROPERTY(State state READ state WRITE setState RESET resetState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
};
Q_DECLARE_METATYPE(NewestPacksModel)

#endif // NEWESTPACKSMODEL_H
