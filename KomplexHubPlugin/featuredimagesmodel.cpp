#include "featuredimagesmodel.h"

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
        &PaginationNotifier::pageChanged,
        this,
        &FeaturedImagesModel::resetDataModel
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetchComplete,
        this,
        &FeaturedImagesModel::resetState
    );

    QObject::connect(
        notifier,
        &PaginationNotifier::fetching,
        this,
        &FeaturedImagesModel::onPaginatorFetching
    );
}

FeaturedImagesModel::~FeaturedImagesModel()
{
    if(m_paginator)
    {
        delete m_paginator;
    }
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
    if(!boundaryCheck(index.row()))
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
        data = dataPoint.sources.value(QString::fromUtf8("thumbnail"));
        break;
    case LargeThumbnailRole:
        data = dataPoint.sources.value(QString::fromUtf8("largeThumbnail"));
        break;
    case PortraitUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("portrait"));
        break;
    case PortraitSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("portrait"));
        break;
    case ScreenUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("screen"));
        break;
    case ScreenSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("screen"));
        break;
    case OriginalUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("original"));
        break;
    case LandscapeUrlRole:
        data = dataPoint.sources.value(QString::fromUtf8("landscape"));
        break;
    case LandscapeSizeRole:
        data = dataPoint.sourceSizes.value(QString::fromUtf8("landscape"));
        break;
    case BackgroundPortraitRole:
        data = dataPoint.sources.value(QString::fromUtf8("backgroundPortrait"));
    case BackgroundLandscapeRole:
        data = dataPoint.sources.value(QString::fromUtf8("backgroundLandscape"));
        break;
    }

    return data;
}

auto FeaturedImagesModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(!boundaryCheck(row))
    {
        return {};
    }

    return createIndex(row, column, &m_paginator[row]);
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
    beginRemoveRows(QModelIndex(), 0, m_paginator->count());
    endRemoveRows();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_paginator->count() - 1);
    endInsertRows();
}

auto FeaturedImagesModel::onPaginatorFetching() -> void
{
    setState(Loading);
}

auto FeaturedImagesModel::boundaryCheck(qsizetype index) const -> bool
{
    if(index < 0 || m_paginator == nullptr || index >= m_paginator->count())
    {
        return false;
    }

    return true;
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

auto FeaturedImagesModel::totalResults() const -> qsizetype
{
    if(m_paginator != nullptr)
    {
        return m_paginator->totalResults();
    }

    return 0;
}

auto FeaturedImagesModel::nextPage() -> void
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

auto FeaturedImagesModel::previousPage() -> void
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
