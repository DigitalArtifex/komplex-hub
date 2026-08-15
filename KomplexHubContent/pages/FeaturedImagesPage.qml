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

    property string query
    readonly property bool searchable: paginator.searchable
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    color: palette.base

    FeaturedImagesModel
    {
        id: searchModel
        //query: rootItem.query
        resultsPerPage: paginator.resultsPerPage

        Component.onCompleted: () => nextPage()
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
            text: qsTr("Featured Images")
            verticalAlignment: Qt.AlignVCenter
        }

        Text
        {
            Layout.alignment: Qt.AlignTop
            Layout.preferredHeight: 50

            color: palette.text
            font.pixelSize: Constants.h4Font.pixelSize
            font.bold: true
            text: qsTr("A specially curated collection of images, courtesy of Pexels")
            verticalAlignment: Qt.AlignVCenter
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        }

        PaginatorGrid
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: paginator
            model: searchModel
            loading: model.state === FeaturedImagesModel.Loading

            delegate: ItemDelegate
            {
                id: resultDelegate

                width: resultWidth
                height: resultHeight

                required property string author
                required property string description
                required property string uuid
                required property string thumbnail
                required property string authorId
                required property int index

                SearchResultItem
                {
                    anchors.fill: parent
                    title: parent.author
                    description: parent.description
                    thumbnail: parent.thumbnail
                    uuid: parent.uuid
                    pexels: true
                    selected: paginator.currentIndex === parent.index

                    onTriggered: () => paginator.currentIndex = parent.index

                    onViewMoreTriggered: () =>
                    {
                        showImagePopup(parent.index)
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

        ImageView
        {
            id: viewMoreImagePopup
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

    function showImagePopup(index)
    {
        viewMoreImagePopup.author = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.AuthorRole)
        viewMoreImagePopup.authorUrl = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.AuthorUrlRole)
        viewMoreImagePopup.description = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.DescriptionRole)
        viewMoreImagePopup.uuid = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.UuidRole)
        viewMoreImagePopup.thumbnail = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LargeThumbnailRole)
        viewMoreImagePopup.original = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.OriginalUrlRole)
        viewMoreImagePopup.portrait = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.PortraitUrlRole)
        viewMoreImagePopup.landscape = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LandscapeUrlRole)
        viewMoreImagePopup.backgroundPortrait = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.BackgroundPortraitRole)
        viewMoreImagePopup.backgroundLandscape = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.BackgroundLandscapeRole)
        viewMoreImagePopup.fullScreen = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ScreenUrlRole)
        viewMoreImagePopup.portraitSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.PortraitSizeRole)
        viewMoreImagePopup.landscapeSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LandscapeSizeRole)
        viewMoreImagePopup.fullScreenSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ScreenSizeRole)
        viewMoreImagePopup.opacity = 1
        popupContainer.opacity = 1
        resultsLayout.opacity = 0
        rootItem.popup = true
    }

    function closePopup()
    {
        viewMoreImagePopup.opacity = 0
        popupContainer.opacity = 0
        resultsLayout.opacity = 1
        rootItem.popup = false
    }
}
