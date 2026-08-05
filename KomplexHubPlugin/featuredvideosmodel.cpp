#include "featuredvideosmodel.h"
#include "common/videocache.h"

FeaturedVideosModel::FeaturedVideosModel(QObject *parent) : QAbstractListModel{parent}
{
    m_itemModel = new VideoItemModel(this);

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
        &PaginationNotifier::pageChanged,
        this,
        &FeaturedVideosModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &FeaturedVideosModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &FeaturedVideosModel::onPaginatorFetching
    );
}

FeaturedVideosModel::~FeaturedVideosModel()
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

auto FeaturedVideosModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(!boundaryCheck(row))
    {
        return {};
    }

    return createIndex(row, column, &m_paginator[row]);
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
    beginRemoveRows(QModelIndex(), 0, m_paginator->count());
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto FeaturedVideosModel::onPaginatorFetching() -> void
{
    setState(Loading);
}

auto FeaturedVideosModel::boundaryCheck(qsizetype index) const -> bool
{
    if(index < 0 || m_paginator == nullptr || index >= m_paginator->count())
    {
        return false;
    }

    return true;
}

auto FeaturedVideosModel::itemModel() const -> VideoItemModel*
{
    return m_itemModel;
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

auto FeaturedVideosModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto FeaturedVideosModel::nextPage() -> void
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

auto FeaturedVideosModel::previousPage() -> void
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

auto FeaturedVideosModel::setItem(qint64 index) -> bool
{
    if(!boundaryCheck(index))
    {
        return false;
    }

    m_itemModel->setDataEntry(m_paginator->at(index));
    return true;
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
