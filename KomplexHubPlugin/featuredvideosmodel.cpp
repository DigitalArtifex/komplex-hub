#include "featuredvideosmodel.h"
#include "common/videocache.h"

FeaturedVideosModel::FeaturedVideosModel(QObject *parent) : QAbstractListModel{parent}
{
    m_paginator = new FeaturedVideosPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &FeaturedVideosModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &FeaturedVideosModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &FeaturedVideosModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &FeaturedVideosModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::dataChanged,
        this,
        &FeaturedVideosModel::resetDataModel
    );
}

auto FeaturedVideosModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto FeaturedVideosModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(index.row() < 0 || m_paginator == nullptr)
    {
        return {};
    }

    VideoCache dataPoint = m_data.at(index.row());

    QVariant data;

    switch(static_cast<DataRole>(role))
    {
        case UuidRole:
            data = dataPoint.id;
            break;
        case AuthorRole:
            data = dataPoint.author;
            break;
        case AuthorIdRole:
            data = dataPoint.authorId;
            break;
        case AuthorUrlRole:
            data = dataPoint.authorUrl;
            break;
        case DurationRole:
            data = dataPoint.duration;
            break;
        case ThumbnailRole:
            data = dataPoint.thumbnail;
            break;
        case UrlRole:
            data = dataPoint.url;
            break;
        case HeightRole:
            data = dataPoint.height;
            break;
        case WidthRole:
            data = dataPoint.width;
            break;
        }

    return data;
}

auto FeaturedVideosModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_data.count())
        return {};

    return createIndex(row, column, &m_data[row]);
}

auto FeaturedVideosModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto FeaturedVideosModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto FeaturedVideosModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto FeaturedVideosModel::state() const -> FeaturedVideosModel::State
{
    return m_state;
}

auto FeaturedVideosModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto FeaturedVideosModel::resetState() -> void
{
    setState(Idle);
}

auto FeaturedVideosModel::resetDataModel() -> void
{
    //invalidate previous model data
    beginRemoveRows(QModelIndex(), 0, m_data.count());
    m_data.clear();
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    auto dataset = m_paginator->data();

    for(const VideoCache& data : std::as_const(dataset))
    {
        m_data.append(data);
    }

    endInsertRows();
}

auto FeaturedVideosModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto FeaturedVideosModel::errorString() const -> QString
{
    return m_errorString;
}

auto FeaturedVideosModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

auto FeaturedVideosModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto FeaturedVideosModel::setResultsPerPage(qsizetype resultsPerPage) -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->setResultsPerPage(resultsPerPage);
    }
}

auto FeaturedVideosModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto FeaturedVideosModel::nextPage() const -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->next();
    }
}

auto FeaturedVideosModel::previousPage() const -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->previous();
    }
}

qsizetype FeaturedVideosModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto FeaturedVideosModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}
