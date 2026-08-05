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
#ifndef NEWESTPACKSMODEL_H
#define NEWESTPACKSMODEL_H
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

#include "paginators/newestpackspaginator.h"

/**
 * @brief The NewestPacksModel class
 */
class KOMPLEX_EXPORT NewestPacksModel : public QAbstractListModel
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
        DescriptionRole,
        NameRole,
        ThumbnailRole,
        CreatedDateRole,
        PriceRole,
        CurrencyRole,
        DownloadCountRole,
        TypeRole
    };
    Q_ENUM(DataRole)

    explicit NewestPacksModel(QObject *parent = nullptr);
    ~NewestPacksModel();

    /**
     * @brief rowCount
     * @param parent
     * @return
     */
    [[nodiscard]]
    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;

    /**
     * @brief data
     * Used by the view to pull data from the model.
     * Required by QAbstractListModel
     * @param index
     * @param role
     * @return
     */
    [[nodiscard]]
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
    [[nodiscard]]
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
    [[nodiscard]]
    auto columnCount(
        const QModelIndex &parent = QModelIndex()
    ) const -> int override;

    /**
     * @brief parent
     * @param index
     * @return
     */
    [[nodiscard]]
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
    [[nodiscard]]
    auto roleNames() const -> QHash<int, QByteArray> override;

    /**
     * @brief errorString
     * @return
     */
    [[nodiscard]]
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
     * @return
     */
    auto totalPages() const -> qsizetype;

    /**
     * @brief nextPage
     */
    Q_INVOKABLE auto nextPage() const -> void;

    /**
     * @brief previousPage
     */
    Q_INVOKABLE auto previousPage() const -> void;

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

    /**
     * @brief resetDataModel
     */
    auto resetDataModel() -> void;

    auto onPaginatorFetching() -> void;

signals:
    auto stateChanged() -> void;
    auto errorStringChanged() -> void;
    auto resultsPerPageChanged() -> void;

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
            static_cast<int>(DescriptionRole),
            QByteArray("description")
        },
        {
            static_cast<int>(NameRole),
            QByteArray("name")
        },
        {
            static_cast<int>(ThumbnailRole),
            QByteArray("thumbnail")
        },
        {
            static_cast<int>(CreatedDateRole),
            QByteArray("createdDate")
        },
        {
            static_cast<int>(PriceRole),
            QByteArray("price")
        },
        {
            static_cast<int>(CurrencyRole),
            QByteArray("currency")
        },
        {
            static_cast<int>(DownloadCountRole),
            QByteArray("downloadCount")
        },
        {
            static_cast<int>(TypeRole),
            QByteArray("type")
        }
    };
    State m_state = Idle;

    QString m_errorString = QString();

    NewestPacksPaginator *m_paginator = nullptr;

    Q_PROPERTY(State state READ state WRITE setState RESET resetState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(qsizetype resultsPerPage READ resultsPerPage WRITE setResultsPerPage NOTIFY resultsPerPageChanged FINAL)
    Q_PROPERTY(qsizetype totalResults READ totalResults NOTIFY totalResultsChanged FINAL)
    Q_PROPERTY(qsizetype totalPages READ totalPages NOTIFY totalPagesChanged FINAL)
    Q_PROPERTY(qsizetype page READ page NOTIFY pageChanged FINAL)
};
Q_DECLARE_METATYPE(NewestPacksModel)

#endif // NEWESTPACKSMODEL_H
