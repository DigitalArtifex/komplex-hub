#include "videosearchmodel.h"

VideoSearchModel::VideoSearchModel(QObject *parent) : QAbstractListModel{parent}
{
    m_itemModel = new VideoItemModel(this);
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
        &PaginationNotifier::pageChanged,
        this,
        &VideoSearchModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &VideoSearchModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &VideoSearchModel::onPaginatorFetching
    );

    QObject::connect(
        m_paginator,
        &VideoSearchPaginator::queryChanged,
        this,
        &VideoSearchModel::queryChanged
    );
}

VideoSearchModel::~VideoSearchModel()
{
    if(m_paginator)
    {
        delete m_paginator;
    }

    if(m_itemModel)
    {
        delete m_itemModel;
    }
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
    if(!boundaryCheck(index.row()))
    {
        return {};
    }

    VideoCache dataPoint = m_paginator->at(index.row());
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

    if(!boundaryCheck(row))
    {
        return {};
    }

    return createIndex(row, column, &m_paginator[row]);
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
    //invalidate previous model data
    if(m_lastCount > 0)
    {
        beginRemoveRows(QModelIndex(), 0, m_lastCount - 1);
        endRemoveRows();
    }

    m_lastCount = m_paginator->count();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto VideoSearchModel::setItem(qint64 index) -> bool
{
    if(!boundaryCheck(index))
    {
        return false;
    }

    m_itemModel->setDataEntry(m_paginator->at(index));
    return true;
}

auto VideoSearchModel::boundaryCheck(qsizetype index) const -> bool
{
    if(index < 0 || m_paginator == nullptr || index >= m_paginator->count())
    {
        return false;
    }

    return true;
}

auto VideoSearchModel::itemModel() const -> VideoItemModel*
{
    return m_itemModel;
}

auto VideoSearchModel::hasNextPage() const -> bool
{
    return m_paginator->page() < m_paginator->totalPages();
}

auto VideoSearchModel::hasPreviousPage() const -> bool
{
    return m_paginator->page() > 0;
}

auto VideoSearchModel::onPaginatorFetching() -> void
{
    setState(Loading);
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
        QFuture<void> future = QtConcurrent::run
        (
            [this, resultsPerPage]
            {
                qsizetype difference = resultsPerPage - m_paginator->resultsPerPage();
                m_paginator->setResultsPerPage(resultsPerPage);
                m_lastCount = m_paginator->resultsPerPage();

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

auto VideoSearchModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto VideoSearchModel::nextPage() -> void
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

auto VideoSearchModel::previousPage() -> void
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
        QFuture<void> future = QtConcurrent::run
        (
            [this, query]
            {
                m_paginator->setQuery(query);
            }
        );
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
