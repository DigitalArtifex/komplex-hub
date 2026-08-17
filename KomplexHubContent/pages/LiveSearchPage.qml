import QtQuick
import QtQuick.Controls
import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin

Item
{
    id: rootItem

    property string query
    readonly property bool searchable: paginator.searchable
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    PackSearchModel
    {
        id: searchModel
        query: rootItem.query
        resultsPerPage: paginator.resultsPerPage
    }

    PaginatorGrid
    {
        id: paginator
        anchors.fill: parent
        model: searchModel
        loading: model.state === PackSearchModel.Loading

        delegate: ItemDelegate
        {
            id: resultDelegate

            width: resultWidth
            height: resultHeight

            required property string name
            required property string author
            required property string description
            required property string uuid
            required property string thumbnail
            required property string authorId
            required property int index

            SearchResultItem
            {
                anchors.fill: parent
                title: parent.name
                author: parent.author
                description: parent.description
                thumbnail: parent.thumbnail
                uuid: parent.uuid
                selected: paginator.currentIndex === parent.index

                onTriggered: () => paginator.currentIndex = parent.index

                onViewMoreTriggered: () =>
                {
                    showWallpaperPopup(parent.index)
                }
            }
        }
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        color: palette.base

        PackView
        {
            id: viewMoreWallpaperPopup
            anchors.fill: parent
            opacity: 0
            visible: opacity > 0.01
            //model: videosModel.itemModel

            Behavior on opacity {
                NumberAnimation
                {
                    duration: 250
                }
            }
        }

        Behavior on opacity {
            NumberAnimation
            {
                duration: 250
            }
        }
    }

    function closePopup()
    {
        viewMoreWallpaperPopup.opacity = 0
        popupContainer.opacity = 0
        paginator.opacity = 1
        rootItem.popup = false
    }

    function showWallpaperPopup(index)
    {
        viewMoreWallpaperPopup.author = searchModel.data(searchModel.index(index,0), PackSearchModel.AuthorRole)
        viewMoreWallpaperPopup.authorId = searchModel.data(searchModel.index(index,0), PackSearchModel.AuthorIdRole)
        viewMoreWallpaperPopup.description = searchModel.data(searchModel.index(index,0), PackSearchModel.DescriptionRole)
        viewMoreWallpaperPopup.thumbnail = searchModel.data(searchModel.index(index,0), PackSearchModel.ThumbnailRole)
        viewMoreWallpaperPopup.opacity = 1
        popupContainer.opacity = 1
        paginator.opacity = 0
        rootItem.popup = true
    }
}
