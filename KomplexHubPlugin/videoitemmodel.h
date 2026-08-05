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
#ifndef VideoItemModel_H
#define VideoItemModel_H
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

#include "common/komplex_global.h"
#include "common/videocache.h"

/**
 * @brief The VideoItemModel class
 */
class KOMPLEX_EXPORT VideoItemModel : public QAbstractListModel
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
        FileTypeRole,
        FpsRole,
        HeightRole,
        WidthRole,
        SizeRole,
        UrlRole,
        QualityRole,
        TextRole
    };
    Q_ENUM(DataRole)

    explicit VideoItemModel(QObject *parent = nullptr);

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
     * @brief setDataEntry
     * Set the dataEntry from the Search or List model
     * @param entry
     */
    auto setDataEntry(const VideoCache &entry) -> void;

    /**
     * @brief friendlyText
     * Generates the friendly text for comboboxes and the like
     * @param video
     * @return
     */
    auto friendlyText(const VideoEntry &video) const -> QString;

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
     * Clears the data model and creates a new one if data exists
     */
    auto resetDataModel() -> void;

signals:
    auto dataEntryChanged() -> void;

    /**
     * @brief stateChanged
     * Signaled when reported state changes
     */
    auto stateChanged() -> void;

    /**
     * @brief errorStringChanged
     * Signaled when reported error message changes
     */
    auto errorStringChanged() -> void;

private:
    /**
     * @brief boundaryCheck
     * Helper function to check the requested index boundary
     * @param index
     * @return
     */
    auto boundaryCheck(qsizetype index) const -> bool;

    /**
     * Size texts used in friendlyText()
     */
    QStringList m_sizeTexts
    {
        QString::fromUtf8("B"),
        QString::fromUtf8("KB"),
        QString::fromUtf8("MB"),
        QString::fromUtf8("GB")
    };

    /**
     * Data role map that connects VideoItemModel::DataRole to it's QML accessor name
     */
    static inline const QHash<int, QByteArray> m_dataRoles =
    {
        {
           static_cast<int>(UuidRole),
           QByteArray("uuid")
        },
        {
           static_cast<int>(FileTypeRole),
           QByteArray("fileType")
        },
        {
           static_cast<int>(FpsRole),
           QByteArray("fps")
        },
        {
           static_cast<int>(HeightRole),
           QByteArray("height")
        },
        {
           static_cast<int>(WidthRole),
           QByteArray("width")
        },
        {
           static_cast<int>(SizeRole),
           QByteArray("size")
        },
        {
           static_cast<int>(UrlRole),
           QByteArray("url")
        },
        {
            static_cast<int>(QualityRole),
            QByteArray("quality")
        },
        {
            static_cast<int>(TextRole),
            QByteArray("text")
        }
    };

    State m_state = Idle;
    QString m_errorString = QString();

    VideoCache m_dataEntry;
    qint64 m_lastCount = 0;

    Q_PROPERTY(State state READ state WRITE setState RESET resetState NOTIFY stateChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
};
Q_DECLARE_METATYPE(VideoItemModel)

#endif // VideoItemModel_H
