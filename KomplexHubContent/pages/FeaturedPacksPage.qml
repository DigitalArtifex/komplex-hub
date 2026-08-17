import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin

Rectangle
{
    id: rootItem

    property string query
    readonly property bool searchable: false
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    color: palette.base

    NewestPacksModel
    {
        id: packsModel
        resultsPerPage: paginator.resultsPerPage

        Component.onCompleted: () => nextPage()
    }

    ColumnLayout
    {
        id: resultsLayout
        visible: opacity > 0.01
        anchors.fill: parent
        anchors.margins: Constants.mediumMargin

        KeroHeader
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: 180
            title: qsTr("Featured Live Wallpapers")
            description: qsTr("Live Wallpaper Packs from Komplex or ShaderToy")
        }

        PaginatorGrid
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: paginator
            model: packsModel
            loading: model.state === NewestPacksModel.Loading

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
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        visible: opacity > 0.01
        color: palette.base

        PackView
        {
            id: viewMoreWallpaperPopup
            anchors.fill: parent
            opacity: 0
            visible: opacity > 0.01

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
        resultsLayout.opacity = 1
        rootItem.popup = false
    }

    function showWallpaperPopup(index)
    {
        viewMoreWallpaperPopup.author = packsModel.data(packsModel.index(index,0), NewestPacksModel.AuthorRole)
        viewMoreWallpaperPopup.authorId = packsModel.data(packsModel.index(index,0), NewestPacksModel.AuthorIdRole)
        viewMoreWallpaperPopup.description = packsModel.data(packsModel.index(index,0), NewestPacksModel.DescriptionRole)
        viewMoreWallpaperPopup.thumbnail = packsModel.data(packsModel.index(index,0), NewestPacksModel.ThumbnailRole)
        viewMoreWallpaperPopup.opacity = 1
        popupContainer.opacity = 1
        resultsLayout.opacity = 0
        rootItem.popup = true
    }
}
