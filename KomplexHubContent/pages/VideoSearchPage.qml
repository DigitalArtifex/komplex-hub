import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin
import KomplexHub.Views
import KomplexHub.Pages

Item
{
    id: rootItem

    property string query
    readonly property bool searchable: paginator.searchable
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    VideoSearchModel
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
        loading: model.state === VideoSearchModel.Loading
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

        VideoView
        {
            id: viewMoreVideoPopup
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

    function showVideoPopup(index)
    {
        searchModel.setItem(index)
        viewMoreVideoPopup.author = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorRole)
        viewMoreVideoPopup.authorUrl = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorUrlRole)
        viewMoreVideoPopup.uuid = searchModel.data(searchModel.index(index,0), VideoSearchModel.UuidRole)
        viewMoreVideoPopup.thumbnail = searchModel.data(searchModel.index(index,0), VideoSearchModel.ThumbnailRole)
        viewMoreVideoPopup.modelIndex = index
        viewMoreVideoPopup.model = searchModel.itemModel
        viewMoreVideoPopup.opacity = 1
        paginator.opacity = 0
        popupContainer.opacity = 1
        rootItem.popup = true
    }

    function closePopup()
    {
        viewMoreVideoPopup.opacity = 0
        popupContainer.opacity = 0
        paginator.opacity = 1
        rootItem.popup = false
    }
}
