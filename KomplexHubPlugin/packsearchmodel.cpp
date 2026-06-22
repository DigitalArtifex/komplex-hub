#include "packsearchmodel.h"

PackSearchModel::PackSearchModel(QObject *parent) : QAbstractListModel{parent}
{
    m_paginator = new PackSearchPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &PackSearchModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &PackSearchModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &PackSearchModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &PackSearchModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &PackSearchModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &PackSearchModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &PackSearchModel::onPaginatorFetching
    );

    QObject::connect(
        m_paginator,
        &PaginationNotifier::queryChanged,
        this,
        &PackSearchModel::queryChanged
    );
}

auto PackSearchModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto PackSearchModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(m_paginator == nullptr)
    {
        return {};
    }

    if(index.row() < 0 || index.row() >= m_paginator->count() )
    {
        return {};
    }

    WallpaperCache dataPoint = m_paginator->at(index.row());

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

auto PackSearchModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_paginator->count())
        return {};

    return createIndex(row, column, &m_paginator[row]);
}

auto PackSearchModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto PackSearchModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto PackSearchModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto PackSearchModel::state() const -> PackSearchModel::State
{
    return m_state;
}

auto PackSearchModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto PackSearchModel::resetState() -> void
{
    setState(Idle);
}

auto PackSearchModel::resetDataModel() -> void
{
    //invalidate previous model data
    beginRemoveRows(QModelIndex(), 0, m_paginator->count());
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto PackSearchModel::hasNextPage() const -> bool
{
    return m_paginator->page() < m_paginator->totalPages();
}

auto PackSearchModel::hasPreviousPage() const -> bool
{
    return m_paginator->page() > 0;
}

auto PackSearchModel::onPaginatorFetching() -> void
{
    setState(Loading);
}

auto PackSearchModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto PackSearchModel::errorString() const -> QString
{
    return m_errorString;
}

auto PackSearchModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

auto PackSearchModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto PackSearchModel::setResultsPerPage(qsizetype resultsPerPage) -> void
{
    if(m_paginator != nullptr)
    {
        QFuture<void> future = QtConcurrent::run
        (
            [this, resultsPerPage]
            {
                m_paginator->setResultsPerPage(resultsPerPage);
            }
        );
    }
}

auto PackSearchModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto PackSearchModel::nextPage() -> void
{
    if(m_paginator != nullptr)
    {
        QFuture<void> future = QtConcurrent::run
        (
            [this]
            {
                m_paginator->next();
            }
        );
    }
}

auto PackSearchModel::previousPage() -> void
{
    if(m_paginator != nullptr)
    {
        QFuture<void> future = QtConcurrent::run
        (
            [this]
            {
                m_paginator->previous();
            }
        );
    }
}

auto PackSearchModel::query() const -> QString
{
    if(m_paginator != nullptr)
    {
        return m_paginator->query();
    }

    return {};
}

auto PackSearchModel::setQuery(const QString &query) -> void
{
    if(m_paginator != nullptr)
    {
        QFuture<void> future = QtConcurrent::run
        (
            [this, query]
            {
                m_paginator->setQuery(query);
            }
        );
    }
}

qsizetype PackSearchModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto PackSearchModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}
