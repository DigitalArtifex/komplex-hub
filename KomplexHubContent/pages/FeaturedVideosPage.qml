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

    FeaturedVideosModel
    {
        id: videosModel
        resultsPerPage: paginator.resultsPerPage
        windowSize: 80
    }

    ColumnLayout
    {
        id: resultsLayout
        visible: opacity > 0.01
        anchors.fill: parent
        anchors.margins: Constants.mediumMargin

        Text
        {
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50

            color: palette.text
            font.pixelSize: Constants.h2Font.pixelSize
            font.bold: true
            text: qsTr("Most Popular Videos")
            verticalAlignment: Qt.AlignVCenter
        }

        Text
        {
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50

            color: palette.text
            font.pixelSize: Constants.h4Font.pixelSize
            font.bold: true
            text: qsTr("Most popular videos, provided courtesy of Pexels")
            verticalAlignment: Qt.AlignVCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        PaginatorGrid
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: paginator
            model: videosModel
            loading: videosModel.state === FeaturedVideosModel.Loading
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
                    description: qsTr("")
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
            id: videoDetailsPopup
            anchors.fill: parent
            opacity: 0
            visible: opacity > 0.01

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
        videosModel.nextPage()
    }

    function showVideoPopup(index)
    {
        videosModel.setItem(index)
        videoDetailsPopup.author = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorRole)
        videoDetailsPopup.authorUrl = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorUrlRole)
        videoDetailsPopup.uuid = searchModel.data(searchModel.index(index,0), VideoSearchModel.UuidRole)
        videoDetailsPopup.thumbnail = searchModel.data(searchModel.index(index,0), VideoSearchModel.ThumbnailRole)
        videoDetailsPopup.modelIndex = index
        videoDetailsPopup.model = searchModel.itemModel
        videoDetailsPopup.opacity = 1
        resultsLayout.opacity = 0
        popupContainer.opacity = 1
        rootItem.popup = true
    }

    function closePopup()
    {
        videoDetailsPopup.opacity = 0
        popupContainer.opacity = 0
        resultsLayout.opacity = 1
        rootItem.popup = false
    }
}
