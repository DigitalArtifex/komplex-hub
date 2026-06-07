#include "newestpacksmodel.h"
#include "common/coreservices.h"
#include "common/logging.h"

NewestPacksModel::NewestPacksModel(QObject *parent) : QAbstractListModel{parent}
{

}

auto NewestPacksModel::rowCount(const QModelIndex &) const -> int
{
    return m_wallpaperCache.count();
}

auto NewestPacksModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(index.row() < 0)
    {
        return {};
    }

    WallpaperCache *dataPoint = m_wallpaperCache.at(index.row());

    if(dataPoint == nullptr)
    {
        return {};
    }

    QVariant data;

    switch(static_cast<DataRole>(role))
    {
        case UriRole:
            data = dataPoint->uri;
            break;
        case AuthorRole:
            data = dataPoint->author;
            break;
        case DescriptionRole:
            data = dataPoint->description;
            break;
        case NameRole:
            data = dataPoint->name;
            break;
        case ThumbnailRole:
            data = dataPoint->thumbnail;
            break;
        case CreatedDateRole:
            data = dataPoint->createdDate;
            break;
        case AuthorIdRole:
            data = dataPoint->authorId;
            break;
        case PriceRole:
            data = dataPoint->price;
            break;
        case CurrencyRole:
            data = dataPoint->currency;
            break;
        case DownloadCountRole:
            data = dataPoint->downloadCount;
            break;
        case TypeRole:
            data = dataPoint->type;
            break;
    }

    return data;
}

auto NewestPacksModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    return createIndex(row, column, m_wallpaperCache.at(row));
}

auto NewestPacksModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto NewestPacksModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto NewestPacksModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return true;
}

auto NewestPacksModel::state() const -> NewestPacksModel::State
{
    return m_state;
}

auto NewestPacksModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto NewestPacksModel::resetState() -> void
{
    setState(Idle);
}

auto NewestPacksModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto NewestPacksModel::errorString() const -> QString
{
    return m_errorString;
}

auto NewestPacksModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}
