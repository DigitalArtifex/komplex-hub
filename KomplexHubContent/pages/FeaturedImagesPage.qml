import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Rectangle
{
    id: rootItem

    readonly property bool searchable: false
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    color: palette.base

    FeaturedImagesModel
    {
        id: imagesModel
        resultsPerPage: paginator.resultsPerPage
        // windowSize: 80
    }

    PaginatorGrid
    {
        id: paginator
        anchors.fill: parent
        model: imagesModel
        loading: imagesModel.state === FeaturedImagesModel.Loading
        visible: opacity > 0.01

        delegate: ItemDelegate
        {
            id: resultDelegate

            width: resultWidth
            height: resultHeight

            required property string uuid
            required property string author
            required property string authorId
            required property string authorUrl
            required property string thumbnail
            required property int index

            SearchResultItem
            {
                anchors.fill: parent
                author: parent.author
                description: qsTr("No description available")
                thumbnail: parent.thumbnail
                uuid: parent.uuid
                pexels: true
                selected: paginator.currentIndex === parent.index

                onTriggered: () => paginator.currentIndex = parent.index

                onViewMoreTriggered: () =>
                {
                    showVideoPopup(parent.index)
                }
            }
        }
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        visible: opacity > 0.01
        color: palette.base

        ImageView
        {
            id: imageDetailsPopup
            anchors.fill: parent
            opacity: 0

            Behavior on opacity
            {
                NumberAnimation
                {
                    duration: 250
                }
            }
        }

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 250
            }
        }
    }

    Component.onCompleted: () =>
    {
        imagesModel.nextPage()
    }

    function showVideoPopup(index)
    {
        imagesModel.setItem(index)
        imageDetailsPopup.author = searchModel.data(searchModel.index(index,0), ImageSearchModel.AuthorRole)
        imageDetailsPopup.authorUrl = searchModel.data(searchModel.index(index,0), ImageSearchModel.AuthorUrlRole)
        imageDetailsPopup.uuid = searchModel.data(searchModel.index(index,0), ImageSearchModel.UuidRole)
        imageDetailsPopup.thumbnail = searchModel.data(searchModel.index(index,0), ImageSearchModel.ThumbnailRole)
        imageDetailsPopup.modelIndex = index
        imageDetailsPopup.model = searchModel.itemModel
        imageDetailsPopup.opacity = 1
        paginator.opacity = 0
        popupContainer.opacity = 1
        rootItem.popup = true
    }

    function closePopup()
    {
        imageDetailsPopup.opacity = 0
        popupContainer.opacity = 0
        paginator.opacity = 1
        rootItem.popup = false
    }
}
