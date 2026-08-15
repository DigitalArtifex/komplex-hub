#include "featuredpacksmodel.h"
#include "common/wallpapercache.h"

FeaturedPacksModel::FeaturedPacksModel(QObject *parent) : QAbstractListModel{parent}
{
    m_paginator = new FeaturedPacksPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &FeaturedPacksModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &FeaturedPacksModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &FeaturedPacksModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &FeaturedPacksModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &FeaturedPacksModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &FeaturedPacksModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &FeaturedPacksModel::onPaginatorFetching
    );
}

FeaturedPacksModel::~FeaturedPacksModel()
{
    if(m_paginator)
    {
        delete m_paginator;
    }
}

auto FeaturedPacksModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto FeaturedPacksModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(index.row() < 0 || m_paginator == nullptr || index.row() >= m_paginator->count())
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

auto FeaturedPacksModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_paginator->count())
    {
        return {};
    }

    return createIndex(row, column, &m_paginator[row]);
}

auto FeaturedPacksModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto FeaturedPacksModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto FeaturedPacksModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto FeaturedPacksModel::state() const -> FeaturedPacksModel::State
{
    return m_state;
}

auto FeaturedPacksModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    Q_EMIT stateChanged();
}

auto FeaturedPacksModel::resetState() -> void
{
    setState(Idle);
}

auto FeaturedPacksModel::resetDataModel() -> void
{
    //invalidate previous model data
    beginRemoveRows(QModelIndex(), 0, m_paginator->count());
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto FeaturedPacksModel::onPaginatorFetching() -> void
{
    setState(Loading);
}

auto FeaturedPacksModel::boundaryCheck(qsizetype index) const -> bool
{
    if(index < 0 || m_paginator == nullptr || index >= m_paginator->count())
    {
        return false;
    }

    return true;
}

auto FeaturedPacksModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto FeaturedPacksModel::errorString() const -> QString
{
    return m_errorString;
}

auto FeaturedPacksModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    Q_EMIT errorStringChanged();
}

auto FeaturedPacksModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto FeaturedPacksModel::setResultsPerPage(qsizetype resultsPerPage) -> void
{
    if(m_paginator != nullptr)
    {
        QFuture<void> future = QtConcurrent::run
        (
            [this, resultsPerPage]
            {
                qsizetype difference = resultsPerPage - m_paginator->resultsPerPage();
                m_paginator->setResultsPerPage(resultsPerPage);

                if(difference > 0)
                {
                    qsizetype firstIndex = m_paginator->resultsPerPage() - difference;

                    beginInsertRows
                    (
                        QModelIndex(),
                        firstIndex,
                        m_paginator->count() - 1
                    );

                    endInsertRows();
                }

                else if(difference < 0)
                {
                    qsizetype firstIndex = m_paginator->resultsPerPage();

                    beginRemoveRows
                    (
                        QModelIndex(),
                        firstIndex,
                        firstIndex - difference
                    );

                    endRemoveRows();
                }
            }
        );
    }
}

auto FeaturedPacksModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto FeaturedPacksModel::nextPage() -> void
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

auto FeaturedPacksModel::previousPage() -> void
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

auto FeaturedPacksModel::hasNextPage() const -> bool
{
    if(m_paginator == nullptr)
    {
        return false;
    }

    return m_paginator->page() < m_paginator->totalPages();
}

auto FeaturedPacksModel::hasPreviousPage() const -> bool
{
    if(m_paginator == nullptr)
    {
        return false;
    }

    return m_paginator->page() > 0;
}

qsizetype FeaturedPacksModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto FeaturedPacksModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}
