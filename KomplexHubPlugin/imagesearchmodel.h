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
#ifndef ImageSearchModel_H
#define ImageSearchModel_H
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

#include "paginators/imagesearchpaginator.h"

/**
 * @brief The ImageSearchModel class
 */
class KOMPLEX_EXPORT ImageSearchModel : public QAbstractListModel
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
        DescriptionRole,
        ThumbnailRole,
        PortraitUrlRole,
        LandscapeUrlRole,
        SmallUrlRole,
        OriginalUrlRole,
        MediumUrlRole,
        LargeUrlRole,
        ExtraLargeUrlRole
    };
    Q_ENUM(DataRole)

    explicit ImageSearchModel(QObject *parent = nullptr);

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
    Q_INVOKABLE auto nextPage() const -> void;

    /**
     * @brief previousPage
     * Function for the QML frontend
     */
    Q_INVOKABLE auto previousPage() const -> void;

    /**
     * @brief query
     * Current search query
     * @return
     */
    auto query() const -> QString;

    /**
     * @brief setQuery
     * Sets the current
     * @param query
     */
    auto setQuery(const QString &query) -> void;

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

    auto hasNextPage() -> bool;

    auto hasPreviousPage() -> bool;

signals:
    auto stateChanged() -> void;
    auto errorStringChanged() -> void;
    auto resultsPerPageChanged() -> void;

    auto pageChanged() -> void;
    auto totalResultsChanged() -> void;
    auto totalPagesChanged() -> void;
    auto queryChanged() -> void;

private:
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
            static_cast<int>(DescriptionRole),
            QByteArray("description")
        },
        {
            static_cast<int>(ThumbnailRole),
            QByteArray("thumbnail")
        },
        {
            static_cast<int>(PortraitUrlRole),
            QByteArray("portrait")
        },
        {
            static_cast<int>(LandscapeUrlRole),
            QByteArray("landscape")
        },
        {
            static_cast<int>(SmallUrlRole),
            QByteArray("small")
        },
        {
            static_cast<int>(OriginalUrlRole),
            QByteArray("original")
        },
        {
            static_cast<int>(MediumUrlRole),
            QByteArray("medium")
        },
        {
            static_cast<int>(LargeUrlRole),
            QByteArray("large")
        },
        {
            static_cast<int>(ExtraLargeUrlRole),
            QByteArray("extraLarge")
        }
    };
    State m_state = Idle;

    QString m_errorString = QString();

    ImageSearchPaginator *m_paginator = nullptr;

    mutable QList<ImageCache> m_data;

    Q_PROPERTY(State state READ state WRITE setState RESET resetState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(qsizetype resultsPerPage READ resultsPerPage WRITE setResultsPerPage NOTIFY resultsPerPageChanged FINAL)
    Q_PROPERTY(qsizetype totalResults READ totalResults NOTIFY totalResultsChanged FINAL)
    Q_PROPERTY(qsizetype totalPages READ totalPages NOTIFY totalPagesChanged FINAL)
    Q_PROPERTY(qsizetype page READ page NOTIFY pageChanged FINAL)
    Q_PROPERTY(bool hasNextPage READ hasNextPage NOTIFY pageChanged FINAL)
    Q_PROPERTY(bool hasPreviousPage READ hasPreviousPage NOTIFY pageChanged FINAL)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged FINAL)
};
Q_DECLARE_METATYPE(ImageSearchModel)

#endif // ImageSearchModel_H
