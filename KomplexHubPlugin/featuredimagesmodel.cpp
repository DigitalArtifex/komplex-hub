#include "featuredimagesmodel.h"
#include "common/wallpapercache.h"

FeaturedImagesModel::FeaturedImagesModel(QObject *parent) : QAbstractListModel{parent}
{
    m_paginator = new FeaturedImagesPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &FeaturedImagesModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &FeaturedImagesModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &FeaturedImagesModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &FeaturedImagesModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::dataChanged,
        this,
        &FeaturedImagesModel::resetDataModel
    );
}

auto FeaturedImagesModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto FeaturedImagesModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(index.row() < 0 || m_paginator == nullptr)
    {
        return {};
    }

    WallpaperCache dataPoint = m_data.at(index.row());

    QVariant data;

    switch(static_cast<DataRole>(role))
    {
        case UuidRole:
            data = dataPoint.uuid;
            break;
        case AuthorRole:
            data = dataPoint.author;
            break;
        case DescriptionRole:
            data = dataPoint.description;
            break;
        case NameRole:
            data = dataPoint.name;
            break;
        case ThumbnailRole:
            data = dataPoint.thumbnail;
            break;
        case CreatedDateRole:
            data = dataPoint.createdDate;
            break;
        case AuthorIdRole:
            data = dataPoint.authorId;
            break;
        case PriceRole:
            data = dataPoint.price;
            break;
        case CurrencyRole:
            data = dataPoint.currency;
            break;
        case DownloadCountRole:
            data = dataPoint.downloadCount;
            break;
        case TypeRole:
            data = dataPoint.type;
            break;
    }

    return data;
}

auto FeaturedImagesModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_data.count())
        return {};

    return createIndex(row, column, &m_data[row]);
}

auto FeaturedImagesModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto FeaturedImagesModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto FeaturedImagesModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto FeaturedImagesModel::state() const -> FeaturedImagesModel::State
{
    return m_state;
}

auto FeaturedImagesModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto FeaturedImagesModel::resetState() -> void
{
    setState(Idle);
}

auto FeaturedImagesModel::resetDataModel() -> void
{
    //invalidate previous model data
    beginRemoveRows(QModelIndex(), 0, m_data.count());
    m_data.clear();
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    auto dataset = m_paginator->data();

    for(const WallpaperCache& data : std::as_const(dataset))
    {
        m_data.append(data);
    }

    endInsertRows();
}

auto FeaturedImagesModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto FeaturedImagesModel::errorString() const -> QString
{
    return m_errorString;
}

auto FeaturedImagesModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

auto FeaturedImagesModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto FeaturedImagesModel::setResultsPerPage(qsizetype resultsPerPage) -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->setResultsPerPage(resultsPerPage);
    }
}

auto FeaturedImagesModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto FeaturedImagesModel::nextPage() const -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->next();
    }
}

auto FeaturedImagesModel::previousPage() const -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->previous();
    }
}

qsizetype FeaturedImagesModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto FeaturedImagesModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}
