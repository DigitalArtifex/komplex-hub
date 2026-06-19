#include "videosearchmodel.h"

VideoSearchModel::VideoSearchModel(QObject *parent) : QAbstractListModel{parent}
{
    setState(Loading);

    m_paginator = new VideoSearchPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &VideoSearchModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &VideoSearchModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &VideoSearchModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &VideoSearchModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::dataChanged,
        this,
        &VideoSearchModel::resetDataModel
    );

    QObject::connect(
        m_paginator,
        &VideoSearchPaginator::queryChanged,
        this,
        &VideoSearchModel::queryChanged
    );
}

auto VideoSearchModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto VideoSearchModel::data(const QModelIndex &index, int role) const -> QVariant
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

auto VideoSearchModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_data.count())
        return {};

    return createIndex(row, column, &m_data[row]);
}

auto VideoSearchModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto VideoSearchModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto VideoSearchModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto VideoSearchModel::state() const -> VideoSearchModel::State
{
    return m_state;
}

auto VideoSearchModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto VideoSearchModel::resetState() -> void
{
    setState(Idle);
}

auto VideoSearchModel::resetDataModel() -> void
{
    setState(Loading);

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

    setState(Idle);
}

auto VideoSearchModel::hasNextPage() -> bool
{
    return m_paginator->page() < m_paginator->totalPages();
}

auto VideoSearchModel::hasPreviousPage() -> bool
{
    return m_paginator->page() > 0;
}

auto VideoSearchModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto VideoSearchModel::errorString() const -> QString
{
    return m_errorString;
}

auto VideoSearchModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

auto VideoSearchModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto VideoSearchModel::setResultsPerPage(qsizetype resultsPerPage) -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->setResultsPerPage(resultsPerPage);
    }
}

auto VideoSearchModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto VideoSearchModel::nextPage() const -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->next();
    }
}

auto VideoSearchModel::previousPage() const -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->previous();
    }
}

auto VideoSearchModel::query() const -> QString
{
    if(m_paginator != nullptr)
    {
        return m_paginator->query();
    }

    return {};
}

auto VideoSearchModel::setQuery(const QString &query) -> void
{
    if(m_paginator != nullptr)
    {
        m_paginator->setQuery(query);
    }
}

qsizetype VideoSearchModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto VideoSearchModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}
