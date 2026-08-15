#include "newestpacksmodel.h"
#include "common/wallpapercache.h"

NewestPacksModel::NewestPacksModel(QObject *parent) : QAbstractListModel{parent}
{
    setState(Loading);

    m_paginator = new NewestPacksPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &NewestPacksModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &NewestPacksModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &NewestPacksModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &NewestPacksModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &NewestPacksModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &NewestPacksModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &NewestPacksModel::onPaginatorFetching
    );
}

NewestPacksModel::~NewestPacksModel()
{
    if(m_paginator)
    {
        delete m_paginator;
    }
}

auto NewestPacksModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto NewestPacksModel::data(const QModelIndex &index, int role) const -> QVariant
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

auto NewestPacksModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_paginator->count())
        return {};

    return createIndex(row, column, &m_paginator[row]);
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
    return false;
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

auto NewestPacksModel::resetDataModel() -> void
{
    //invalidate previous model data
    beginRemoveRows(QModelIndex(), 0, m_paginator->count());
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto NewestPacksModel::onPaginatorFetching() -> void
{
    setState(Loading);
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

auto NewestPacksModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto NewestPacksModel::setResultsPerPage(qsizetype resultsPerPage) -> void
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

auto NewestPacksModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto NewestPacksModel::nextPage() const -> void
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

auto NewestPacksModel::previousPage() const -> void
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

qsizetype NewestPacksModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto NewestPacksModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}

auto NewestPacksModel::hasNextPage() const -> bool
{
    if(m_paginator == nullptr)
    {
        return false;
    }

    return m_paginator->page() < m_paginator->totalPages();

}

auto NewestPacksModel::hasPreviousPage() const -> bool
{
    if(m_paginator == nullptr)
    {
        return false;
    }

    return m_paginator->page() > 0;
}
