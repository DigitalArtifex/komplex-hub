#include "imagesearchmodel.h"

ImageSearchModel::ImageSearchModel(QObject *parent) : QAbstractListModel{parent}
{
    m_paginator = new ImageSearchPaginator(this);
    PaginationNotifier *notifier = static_cast<PaginationNotifier*>(m_paginator);

    QObject::connect(
        notifier,
        &PaginationNotifier::resultsPerPageChanged,
        this,
        &ImageSearchModel::resultsPerPageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalResultsChanged,
        this,
        &ImageSearchModel::totalResultsChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::totalPagesChanged,
        this,
        &ImageSearchModel::totalPagesChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &ImageSearchModel::pageChanged
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::pageChanged,
        this,
        &ImageSearchModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &ImageSearchModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &ImageSearchModel::onPaginatorFetching
    );

    QObject::connect(
        m_paginator,
        &ImageSearchPaginator::queryChanged,
        this,
        &ImageSearchModel::queryChanged
    );
}

ImageSearchModel::~ImageSearchModel()
{
    if(m_paginator != nullptr)
    {
        delete m_paginator;
    }
}

auto ImageSearchModel::rowCount(const QModelIndex &) const -> int
{
    if(m_paginator != nullptr)
    {
        return m_paginator->count();
    }

    return 0;
}

auto ImageSearchModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(index.row() < 0 || m_paginator == nullptr || index.row() >= m_paginator->count())
    {
        return {};
    }

    ImageCache dataPoint = m_paginator->at(index.row());

    QVariant data;

    switch(static_cast<DataRole>(role))
    {
    case UuidRole:
        data = dataPoint.id;
        break;
    case AuthorRole:
        data = dataPoint.photographer;
        break;
    case AuthorIdRole:
        data = dataPoint.photographerId;
        break;
    case AuthorUrlRole:
        data = dataPoint.photographerUrl;
        break;
    case DescriptionRole:
        data = dataPoint.altText;
        break;
    case ThumbnailRole:
        data = dataPoint.sources.value(QString::fromUtf8("tiny"));
        break;
    case PortraitUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("portrait"));
        break;
    case LandscapeUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("landscape"));
        break;
    case SmallUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("small"));
        break;
    case OriginalUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("original"));
        break;
    case MediumUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("medium"));
        break;
    case LargeUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("large"));
        break;
    case ExtraLargeUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("large2x"));
        break;
    case PortraitSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("portrait"));
        break;
    case LandscapeSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("landscape"));
        break;
    case SmallSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("small"));
        break;
    case OriginalSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("original"));
        break;
    case MediumSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("medium"));
        break;
    case LargeSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("large"));
        break;
    case ExtraLargeSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("large2x"));
        break;
    }

    return data;
}

auto ImageSearchModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(row < 0 || row >= m_paginator->count())
    {
        return {};
    }

    return createIndex(row, column, &m_paginator->at(row));
}

auto ImageSearchModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto ImageSearchModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto ImageSearchModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto ImageSearchModel::state() const -> ImageSearchModel::State
{
    return m_state;
}

auto ImageSearchModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto ImageSearchModel::resetState() -> void
{
    setState(Idle);
}

auto ImageSearchModel::resetDataModel() -> void
{
    //invalidate previous model data
    beginRemoveRows(QModelIndex(), 0, m_paginator->count());
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto ImageSearchModel::onPaginatorFetching() -> void
{
    setState(Loading);
}

auto ImageSearchModel::hasNextPage() const -> bool
{
    return m_paginator->page() < m_paginator->totalPages();
}

auto ImageSearchModel::hasPreviousPage() const -> bool
{
    return m_paginator->page() > 0;
}

auto ImageSearchModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto ImageSearchModel::errorString() const -> QString
{
    return m_errorString;
}

auto ImageSearchModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}

auto ImageSearchModel::resultsPerPage() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->resultsPerPage();
    }

    return 0;
}

auto ImageSearchModel::setResultsPerPage(qsizetype resultsPerPage) -> void
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

auto ImageSearchModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto ImageSearchModel::nextPage() -> void
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

auto ImageSearchModel::previousPage() -> void
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

auto ImageSearchModel::query() const -> QString
{
    if(m_paginator != nullptr)
    {
        return m_paginator->query();
    }

    return {};
}

auto ImageSearchModel::setQuery(const QString &query) -> void
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

qsizetype ImageSearchModel::page() const
{
    if(m_paginator != nullptr)
    {
        return m_paginator->page();
    }

    return 0;
}

auto ImageSearchModel::totalPages() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalPages();
    }

    return 0;
}
