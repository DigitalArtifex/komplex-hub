/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex
 *  https://digitalartifex.dev - https://github.com/DigitalArtifex
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>
 */
#ifndef FEATUREDVIDEOSMODEL_H
#define FEATUREDVIDEOSMODEL_H
#include <QObject>
#include <QQmlEngine>
#include <QList>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAbstractListModel>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QtConcurrent/QtConcurrentRun>

#include "paginators/featuredvideospaginator.h"
#include "videoitemmodel.h"

/**
 * @brief The FeaturedVideosModel class
 */
class KOMPLEX_EXPORT FeaturedVideosModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:

    /**
     * @brief Used to control the UI state
     */
    enum State
    {
        Idle,
        Loading,
        Error
    };
    Q_ENUM(State)

    /**
     * @brief Data roles used by the view to request data
     */
    enum DataRole {
        UuidRole = Qt::UserRole + 1,
        AuthorRole,
        AuthorIdRole,
        AuthorUrlRole,
        DurationRole,
        ThumbnailRole,
        UrlRole,
        HeightRole,
        WidthRole
    };
    Q_ENUM(DataRole)

    explicit FeaturedVideosModel(QObject *parent = nullptr);
    ~FeaturedVideosModel();

    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;

    /**
     * @brief data
     * Used by the view to pull data from the model.
     * Required by QAbstractListModel
     * @param index
     * @param role
     * @return
     */
    auto data(
        const QModelIndex &index,
        int role = Qt::DisplayRole
    ) const -> QVariant override;

    /**
     * @brief index
     * Used by the view to create an index for the data point.
     * Required by QAbstractListModel
     * @param row
     * @param column
     * @param parent
     * @return
     */
    auto index(
        int row,
        int column,
        const QModelIndex &parent = QModelIndex()
    ) const -> QModelIndex override;

    /**
     * @brief columnCount
     * Required by QAbstractListModel, but just returns 0
     * since we dont use cols
     * @param parent
     * @return
     */
    auto columnCount(
        const QModelIndex &parent = QModelIndex()
    ) const -> int override;

    /**
     * @brief parent
     * @param index
     * @return
     */
    auto parent(const QModelIndex &index) const -> QModelIndex override;

    /**
     * @brief setData
     * Required by QAbstractListModel but not used
     * @param index
     * @param value
     * @param role
     * @return true always
     */
    auto setData(
        const QModelIndex &index,
        const QVariant &value,
        int role = Qt::EditRole
    ) -> bool override;

    /**
     * @brief state
     * Current Model State
     * @return
     */
    auto state() const -> State;

    /**
     * @brief roleNames
     * Used to tell the view what data roles are available
     * and their QML friendly names
     * @return
     */
    auto roleNames() const -> QHash<int, QByteArray> override;

    /**
     * @brief errorString
     * User-friendly error string to be reported to the user
     * @return
     */
    auto errorString() const -> QString;

    /**
     * @brief resultsPerPage
     * @return
     */
    auto resultsPerPage() const -> qsizetype;

    /**
     * @brief setResultsPerPage
     * @param resultsPerPage
     */
    auto setResultsPerPage(qsizetype resultsPerPage) -> void;

    /**
     * @brief totalResults
     * Total number of results available
     * @return
     */
    auto totalResults() const -> qsizetype;

    /**
     * @brief page
     * @return
     */
    auto page() const -> qsizetype;

    /**
     * @brief totalPages
     * Total pages based on the current resultsPerPage
     * @return
     */
    auto totalPages() const -> qsizetype;

    /**
     * @brief nextPage
     * Function for the QML frontend
     */
    Q_INVOKABLE auto nextPage() -> void;

    /**
     * @brief previousPage
     * Function for the QML frontend
     */
    Q_INVOKABLE auto previousPage() -> void;

    /**
     * @brief setItem
     * Sets the current itemModel to the item at the specified index
     * @param index
     * @return
     */
    Q_INVOKABLE auto setItem(qint64 index) -> bool;

    VideoItemModel *itemModel() const;

    /**
     * @brief windowSize
     * Paginator cache size
     * @return
     */
    auto windowSize() const -> qsizetype;

    /**
     * @brief setWindowSize
     * Set the paginator cache size
     * @param size
     */
    auto setWindowSize(qsizetype size) -> void;

    /**
     * @brief hasNextPage
     * Calculates if there are more pages based on the current page calculation
     * @return
     */
    auto hasNextPage() const -> bool;

    /**
     * @brief hasPreviousPage
     * If page is greater than 1, returns true
     * @return
     */
    auto hasPreviousPage() const -> bool;

protected:
    /**
     * @brief setErrorString
     * @param errorString
     */
    auto setErrorString(const QString &errorString) -> void;

    /**
     * @brief setState
     * @param state
     */
    auto setState(State state) -> void;

    /**
     * @brief resetState
     */
    auto resetState() -> void;

    auto resetDataModel() -> void;

    auto onPaginatorFetching() -> void;

signals:
    auto stateChanged() -> void;
    auto errorStringChanged() -> void;
    auto resultsPerPageChanged() -> void;
    auto windowSizeChanged() -> void;
    auto pageChanged() -> void;
    auto totalResultsChanged() -> void;
    auto totalPagesChanged() -> void;

private:
    /**
     * @brief boundaryCheck
     * Helper function to check the requested index boundary
     * @param index
     * @return
     */
    auto boundaryCheck(qsizetype index) const -> bool;

    /**
     * @brief m_dataRoles
     * Data role map that connects ImageSearchModel::DataRole to it's QML accessor name
     */
    static inline const QHash<int, QByteArray> m_dataRoles =
    {
        {
            static_cast<int>(UuidRole),
            QByteArray("uuid")
        },
        {
            static_cast<int>(AuthorRole),
            QByteArray("author")
        },
        {
            static_cast<int>(AuthorIdRole),
            QByteArray("authorId")
        },
        {
            static_cast<int>(AuthorUrlRole),
            QByteArray("authorUrl")
        },
        {
            static_cast<int>(DurationRole),
            QByteArray("duration")
        },
        {
            static_cast<int>(ThumbnailRole),
            QByteArray("thumbnail")
        },
        {
            static_cast<int>(UrlRole),
            QByteArray("url")
        },
        {
            static_cast<int>(HeightRole),
            QByteArray("height")
        },
        {
            static_cast<int>(WidthRole),
            QByteArray("width")
        }
    };
    State m_state = Idle;

    QString m_errorString = QString();

    FeaturedVideosPaginator *m_paginator = nullptr;
    VideoItemModel *m_itemModel = nullptr;

    Q_PROPERTY(State state READ state WRITE setState RESET resetState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(qsizetype resultsPerPage READ resultsPerPage WRITE setResultsPerPage NOTIFY resultsPerPageChanged FINAL)
    Q_PROPERTY(qsizetype totalResults READ totalResults NOTIFY totalResultsChanged FINAL)
    Q_PROPERTY(qsizetype totalPages READ totalPages NOTIFY totalPagesChanged FINAL)
    Q_PROPERTY(qsizetype page READ page NOTIFY pageChanged FINAL)
    Q_PROPERTY(bool hasNextPage READ hasNextPage NOTIFY pageChanged FINAL)
    Q_PROPERTY(bool hasPreviousPage READ hasPreviousPage NOTIFY pageChanged FINAL)
    Q_PROPERTY(VideoItemModel *itemModel READ itemModel CONSTANT FINAL)
    Q_PROPERTY(qsizetype windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged FINAL)
};
Q_DECLARE_METATYPE(FeaturedVideosModel)

#endif // FEATUREDVIDEOSMODEL_H
