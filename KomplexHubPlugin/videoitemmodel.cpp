#include "videoitemmodel.h"

VideoItemModel::VideoItemModel(QObject *parent) : QAbstractListModel{parent}
{
    QObject::connect(
        this,
        &VideoItemModel::dataEntryChanged,
        this,
        &VideoItemModel::resetDataModel
    );
}

auto VideoItemModel::rowCount(const QModelIndex &) const -> int
{
    return m_dataEntry.files.count();
}

auto VideoItemModel::data(const QModelIndex &index, int role) const -> QVariant
{
    if(!boundaryCheck(index.row()))
    {
        return {};
    }

    VideoEntry dataPoint = m_dataEntry.files.at(index.row());

    QVariant data;

    switch(static_cast<DataRole>(role))
    {
        case UuidRole:
            data = dataPoint.id;
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
        case FileTypeRole:
            data = dataPoint.type;
            break;
        case FpsRole:
            data = dataPoint.fps;
            break;
        case SizeRole:
            data = dataPoint.size;
            break;
        case QualityRole:
            data = dataPoint.quality;
            break;
        case TextRole:
            data = friendlyText(dataPoint);
            break;
    }

    return data;
}

auto VideoItemModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex
{
    Q_UNUSED(parent)

    if(!boundaryCheck(row))
    {
        return {};
    }

    return createIndex(row, column, &m_dataEntry.files[row]);
}

auto VideoItemModel::columnCount(const QModelIndex &) const -> int
{
    return 0;
}

auto VideoItemModel::parent(const QModelIndex &) const -> QModelIndex
{
    return {};
}

auto VideoItemModel::setData(const QModelIndex &, const QVariant &, int) -> bool
{
    return false;
}

auto VideoItemModel::state() const -> VideoItemModel::State
{
    return m_state;
}

auto VideoItemModel::setState(State state) -> void
{
    if (m_state == state)
    {
        return;
    }

    m_state = state;
    emit stateChanged();
}

auto VideoItemModel::resetState() -> void
{
    setState(Idle);
}

auto VideoItemModel::resetDataModel() -> void
{
    if(m_lastCount > 0)
    {
        //invalidate previous model data
        beginRemoveRows(QModelIndex(), 0, m_lastCount - 1);
        endRemoveRows();
    }

    m_lastCount = m_dataEntry.files.count();

    //signal new data
    beginInsertRows(QModelIndex(), 0, m_dataEntry.files.count() - 1);
    endInsertRows();
}

auto VideoItemModel::boundaryCheck(qsizetype index) const -> bool
{
    if(index < 0 || index >= m_dataEntry.files.count())
    {
        return false;
    }

    return true;
}

auto VideoItemModel::setDataEntry(const VideoCache &entry) -> void
{
    if(entry == m_dataEntry)
    {
        return;
    }

    m_dataEntry = entry;
    Q_EMIT dataEntryChanged();
}

auto VideoItemModel::friendlyText(const VideoEntry &video) const -> QString
{
    qint64 size = video.size;
    qint64 sizeIndex = 0;

    while(size >= 1000)
    {
        size = static_cast<qint64>(std::ceil(static_cast<qreal>(size) / 1000));
        ++sizeIndex;
    }

    if(!video.quality.isEmpty())
    {
        return QString("%1/%2 [%3x%4 %5FPS] (%6%7)").arg
        (
            video.quality.toUpper(),
            video.type.toUpper(),
            QString::number(video.width),
            QString::number(video.height),
            QString::number(video.fps),
            QString::number(size),
            m_sizeTexts[sizeIndex]
        );
    }

    return QString("%1 [%2x%3 %4FPS] (%5%6)").arg
    (
        video.type.toUpper(),
        QString::number(video.width),
        QString::number(video.height),
        QString::number(video.fps),
        QString::number(size),
        m_sizeTexts[sizeIndex]
    );
}

auto VideoItemModel::roleNames() const -> QHash<int, QByteArray>
{
    return m_dataRoles;
}

auto VideoItemModel::errorString() const -> QString
{
    return m_errorString;
}

auto VideoItemModel::setErrorString(const QString &errorString) -> void
{
    if (m_errorString == errorString)
    {
        return;
    }

    m_errorString = errorString;
    emit errorStringChanged();
}