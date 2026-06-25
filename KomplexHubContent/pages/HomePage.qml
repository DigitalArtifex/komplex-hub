import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Item
{
    readonly property bool searchable: false
    property int resultsPerRow: (homePageRoot.width - (64 + Constants.largeMargin)) / (resultWidth + Constants.largeMargin) + 1
    property int resultWidth: 256
    property int resultHeight: 245
    property bool popup: false

    clip: true

    id: homePageRoot

    NewestPacksModel
    {
        id: packsModel
        resultsPerPage: homePageRoot.resultsPerRow
    }

    FeaturedImagesModel
    {
        id: imagesModel
        resultsPerPage: homePageRoot.resultsPerRow
    }

    FeaturedVideosModel
    {
        id: videosModel
        resultsPerPage: homePageRoot.resultsPerRow
    }

    Rectangle
    {
        anchors.fill: parent
        color: palette.base

        ColumnLayout
        {
            id: resultsLayout
            anchors.fill: parent
            anchors.margins: Constants.mediumMargin

            Text
            {
                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 50

                color: palette.text
                font.pixelSize: Constants.h2Font.pixelSize
                font.bold: true
                text: qsTr("Newest Wallpaper Packs")
                verticalAlignment: Qt.AlignVCenter
            }

            HorizontalPaginator
            {
                id: packsPaginator
                Layout.fillWidth: true
                Layout.preferredHeight: 245
                Layout.alignment: Qt.AlignTop

                model: packsModel
                delegate: ItemDelegate
                {
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

                        selected: packsPaginator.currentIndex === parent.index

                        onTriggered: () => packsPaginator.currentIndex = parent.index

                        function onViewMoreTriggered()
                        {

                        }
                    }
                }
            }

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

            HorizontalPaginator
            {
                id: imagesPaginator
                Layout.fillWidth: true
                Layout.preferredHeight: 245
                Layout.alignment: Qt.AlignTop

                model: imagesModel
                delegate: ItemDelegate
                {
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
                        selected: imagesPaginator.currentIndex === parent.index

                        onTriggered: () => packsPaginator.currentIndex = parent.index

                        onViewMoreTriggered: () =>
                        {
                            showImagePopup(parent.index)
                        }
                    }
                }
            }

            Text
            {
                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 50

                color: palette.text
                font.pixelSize: Constants.h2Font.pixelSize
                font.bold: true
                text: qsTr("Popular Videos")
                verticalAlignment: Qt.AlignVCenter
            }

            HorizontalPaginator
            {
                id: videosPaginator
                Layout.fillWidth: true
                Layout.preferredHeight: 245
                Layout.alignment: Qt.AlignTop

                model: videosModel
                delegate: ItemDelegate
                {
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
                        title: parent.author
                        thumbnail: parent.thumbnail
                        uuid: parent.uuid
                        pexels: true
                        selected: videosPaginator.currentIndex === parent.index

                        onTriggered: () => packsPaginator.currentIndex = parent.index

                        onViewMoreTriggered:() =>
                        {
                            viewMoreImagePopup.opacity = 1
                        }
                    }
                }
            }

            /* spacer */
            Item { Layout.fillHeight: true }
        }
    }

    Component.onCompleted: () =>
    {
        imagesModel.nextPage()
        videosModel.nextPage()
        packsModel.nextPage()
    }

    KeroLoadingAnimation
    {
        id: loadingAnimation
        anchors.fill: parent

        visible: opacity > 0
        opacity: 1
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        color: palette.base

        ImageView
        {
            id: viewMoreImagePopup
            anchors.fill: parent
            opacity: 0

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

    states: [
        State {
            name: "loading"
            when: packsModel.state === NewestPacksModel.Loading ||
                  imagesModel.state === FeaturedImagesModel.Loading ||
                  videosModel.state === FeaturedVideosModel.Loading;

            PropertyChanges
            {
                target: loadingAnimation
                opacity: 1
            }

            PropertyChanges
            {
                target: resultsLayout
                opacity: 0
            }

            PropertyChanges
            {
                target: popupContainer
                opacity: 0
            }
        },
        State {
            name: "idle"
            when: packsModel.state !== NewestPacksModel.Loading &&
                  imagesModel.state !== FeaturedImagesModel.Loading &&
                  videosModel.state !== FeaturedVideosModel.Loading;

            PropertyChanges
            {
                target: loadingAnimation
                opacity: 0
            }

            PropertyChanges
            {
                target: resultsLayout
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition
        {
            from: "loading"
            to: "idle"
            NumberAnimation
            {
                target: loadingAnimation
                duration: 250
                property: "opacity";
            }
            NumberAnimation
            {
                target: resultsLayout
                duration: 250
                property: "opacity";
            }
        }

    ]

    function showImagePopup(index)
    {
        viewMoreImagePopup.author = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.AuthorRole)
        viewMoreImagePopup.authorUrl = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.AuthorUrlRole)
        viewMoreImagePopup.description = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.DescriptionRole)
        viewMoreImagePopup.uuid = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.UuidRole)
        viewMoreImagePopup.thumbnail = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.ThumbnailRole)
        viewMoreImagePopup.small = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.SmallUrlRole)
        viewMoreImagePopup.medium = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.MediumUrlRole)
        viewMoreImagePopup.large = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.LargeUrlRole)
        viewMoreImagePopup.extraLarge = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.ExtraLargeUrlRole)
        viewMoreImagePopup.original = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.OriginalUrlRole)
        viewMoreImagePopup.portrait = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.PortraitUrlRole)
        viewMoreImagePopup.landscape = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.LandscapeUrlRole)
        viewMoreImagePopup.smallSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.SmallSizeRole)
        viewMoreImagePopup.mediumSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.MediumSizeRole)
        viewMoreImagePopup.largeSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.LargeSizeRole)
        viewMoreImagePopup.extraLargeSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.ExtraLargeSizeRole)
        viewMoreImagePopup.originalSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.OriginalSizeRole)
        viewMoreImagePopup.portraitSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.PortraitSizeRole)
        viewMoreImagePopup.landscapeSize = imagesModel.data(imagesModel.index(0,index), FeaturedImagesModel.LandscapeSizeRole)
        viewMoreImagePopup.opacity = 1
        popupContainer.opacity = 1
        homePageRoot.popup = true
    }

    function closePopup()
    {
        viewMoreImagePopup.opacity = 0
        popupContainer.opacity = 0
        homePageRoot.popup = false
    }
}
