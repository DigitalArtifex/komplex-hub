import QtQuick
import QtQuick.Controls 2.0
import QtQuick.Layouts
import QtQuick.Effects

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin
import KomplexHubContent

Item
{
    readonly property bool searchable: false
    property int resultsPerRow: (homePageRoot.width - (64 + Constants.largeMargin)) / (resultWidth + Constants.largeMargin) + 1
    property int resultWidth: 256
    property int resultHeight: 245
    property bool popup: false
    property bool loading: packsModel.state === NewestPacksModel.Loading ||
                           imagesModel.state === FeaturedImagesModel.Loading ||
                           videosModel.state === FeaturedVideosModel.Loading;

    onWidthChanged: () =>
    {
        packsModel.resultsPerPage = resultsPerRow
        imagesModel.resultsPerPage = resultsPerRow
        videosModel.resultsPerPage = resultsPerRow
    }

    id: homePageRoot
    clip: true

    NewestPacksModel
    {
        id: packsModel
        resultsPerPage: 0
    }

    FeaturedImagesModel
    {
        id: imagesModel
        resultsPerPage: 0
    }

    FeaturedVideosModel
    {
        id: videosModel
        resultsPerPage: 0
    }

    Flickable
    {
        id: pageArea
        visible: opacity > 0.01
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        contentHeight: content.height
        contentWidth: content.width

        Column
        {
            id: content
            width: pageArea.width

            VerticalSpacer{ height: Constants.largeMargin; width: Constants.largeMargin }

            Rectangle
            {
                anchors.left: parent.left
                anchors.right: parent.right
                height: childrenRect.height
                anchors.margins: Constants.largeMargin

                color: palette.base
                border.color: palette.midlight

                Column
                {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    KeroHeader
                    {
                        height: 180
                        anchors.left: parent.left
                        anchors.right: parent.right
                    }

                    Rectangle
                    {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: childrenRect.height

                        color: palette.window

                        Column
                        {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.margins: Constants.largeMargin

                            Text
                            {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 36

                                color: palette.text
                                font.pixelSize: Constants.h2Font.pixelSize
                                font.bold: true
                                text: qsTr("Featured Komplex Wallpapers")
                                verticalAlignment: Qt.AlignVCenter
                            }

                            HorizontalPaginator
                            {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 245

                                id: packsPaginator
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

                                        onViewMoreTriggered: () =>
                                        {
                                            showWallpaperPopup(parent.index)
                                        }
                                    }
                                }

                                onViewMoreTriggered: () =>
                                {
                                    showFeaturedPacksPage()
                                }
                            }

                            Text
                            {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 36

                                color: palette.text
                                font.pixelSize: Constants.h2Font.pixelSize
                                font.bold: true
                                text: qsTr("Featured Images")
                                verticalAlignment: Qt.AlignVCenter
                            }

                            HorizontalPaginator
                            {
                                id: imagesPaginator
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 245

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
                                        author: parent.author
                                        description: parent.description
                                        thumbnail: parent.thumbnail
                                        uuid: parent.uuid
                                        pexels: true
                                        selected: imagesPaginator.currentIndex === parent.index

                                        onTriggered: () => imagesPaginator.currentIndex = parent.index

                                        onViewMoreTriggered: () =>
                                        {
                                            showImagePopup(parent.index)
                                        }
                                    }
                                }

                                onViewMoreTriggered: () =>
                                {
                                    showFeaturedImagesPage()
                                }
                            }

                            VerticalSpacer { height: Constants.largeMargin; width: Constants.largeMargin }
                        }
                    }
                }
            }

            VerticalSpacer { height: Constants.largeMargin; width: Constants.largeMargin }

            Rectangle
            {
                anchors.left: parent.left
                anchors.right: parent.right
                height: childrenRect.height
                anchors.margins: Constants.largeMargin

                color: palette.base
                border.color: palette.midlight

                Column
                {
                    anchors.left: parent.left
                    anchors.right: parent.right

                    KeroHeader
                    {
                        height: 180
                        anchors.left: parent.left
                        anchors.right: parent.right
                        title: qsTr("New & Popular Wallpapers")
                        image: "qrc:/images/kero/kero_star.png"
                    }

                    Rectangle
                    {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: Constants.largeMargin

                        height: childrenRect.height
                        color: palette.window

                        Column
                        {
                            anchors.left: parent.left
                            anchors.right: parent.right

                            Text
                            {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 36

                                color: palette.text
                                font.pixelSize: Constants.h2Font.pixelSize
                                font.bold: true
                                text: qsTr("Newest Wallpaper Packs")
                                verticalAlignment: Qt.AlignVCenter
                            }

                            HorizontalPaginator
                            {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 245

                                id: packsPaginator2
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

                                        selected: packsPaginator2.currentIndex === parent.index

                                        onTriggered: () => packsPaginator2.currentIndex = parent.index

                                        onViewMoreTriggered: () =>
                                        {
                                            showWallpaperPopup(parent.index)
                                        }
                                    }
                                }

                                onViewMoreTriggered: () =>
                                {
                                    showFeaturedPacksPage()
                                }
                            }

                            Text
                            {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 36

                                color: palette.text
                                font.pixelSize: Constants.h2Font.pixelSize
                                font.bold: true
                                text: qsTr("Popular Videos")
                                verticalAlignment: Qt.AlignVCenter
                            }

                            HorizontalPaginator
                            {
                                id: videosPaginator

                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: 245

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
                                        author: parent.author
                                        thumbnail: parent.thumbnail
                                        uuid: parent.uuid
                                        pexels: true
                                        selected: videosPaginator.currentIndex === parent.index

                                        onTriggered: () => videosPaginator.currentIndex = parent.index
                                        onViewMoreTriggered:() => showVideoPopup(parent.index)
                                    }
                                }

                                onViewMoreTriggered: () =>
                                {
                                    showFeaturedVideosPage()
                                }
                            }

                            VerticalSpacer{ height: Constants.largeMargin; width: Constants.largeMargin }
                        }
                    }
                }
            }

            VerticalSpacer{ height: Constants.largeMargin; width: Constants.largeMargin }
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
        visible: opacity > 0.01
        color: palette.base

        ImageView
        {
            id: viewMoreImagePopup
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

        VideoView
        {
            id: viewMoreVideoPopup
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

        PackView
        {
            id: viewMoreWallpaperPopup
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

    Component
    {
        id: featuredVideosComponent

        FeaturedVideosPage
        {
            id: featuredVideosPage
            anchors.fill: parent
            opacity: 0

            Behavior on opacity
            {
                NumberAnimation
                {
                    duration: 250
                }
            }

            Component.onCompleted: () => featuredVideosPage.opacity = 1
        }
    }

    Component
    {
        id: featuredPacksComponent

        FeaturedPacksPage
        {
            id: featuredPacksPage
            anchors.fill: parent
            opacity: 0

            Behavior on opacity
            {
                NumberAnimation
                {
                    duration: 250
                }
            }

            Component.onCompleted: () => featuredPacksPage.opacity = 1
        }
    }

    Component
    {
        id: featuredImagesComponent

        FeaturedImagesPage
        {
            id: featuredImagesPage
            anchors.fill: parent
            opacity: 0

            Behavior on opacity
            {
                NumberAnimation
                {
                    duration: 250
                }
            }

            Component.onCompleted: () => featuredImagesPage.opacity = 1
        }
    }

    Item
    {
        id: featuredLoaderItem
        anchors.fill: parent
        opacity: 0
        visible: opacity > 0.01
        enabled: visible

        Loader
        {
            id: featuredLoader
            anchors.fill: parent
        }
    }

    states:
    [
        State
        {
            name: "loading"
            when: homePageRoot.loading

            PropertyChanges
            {
                target: loadingAnimation
                opacity: 1
            }

            PropertyChanges
            {
                target: pageArea
                opacity: 0
            }

            PropertyChanges
            {
                target: popupContainer
                opacity: 0
            }
        },
        State
        {
            name: "idle"
            when: !popup && !loading

            PropertyChanges
            {
                target: loadingAnimation
                opacity: 0
            }

            PropertyChanges
            {
                target: pageArea
                opacity: 1
            }
        },
        State
        {
            name: "popup"
            when: popup

            PropertyChanges
            {
                target: pageArea
                opacity: 0
            }

            PropertyChanges
            {
                target: loadingAnimation
                opacity: 0
            }
        }
    ]

    transitions:
    [
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
                target: pageArea
                duration: 250
                property: "opacity";
            }
        }
    ]

    function showFeaturedVideosPage()
    {
        pageArea.opacity = 0
        featuredLoaderItem.opacity = 1
        featuredLoader.sourceComponent = featuredVideosComponent
        popup = true
    }

    function showFeaturedPacksPage()
    {
        pageArea.opacity = 0
        featuredLoaderItem.opacity = 1
        featuredLoader.sourceComponent = featuredPacksComponent
        popup = true
    }

    function showFeaturedImagesPage()
    {
        pageArea.opacity = 0
        featuredLoaderItem.opacity = 1
        featuredLoader.sourceComponent = featuredImagesComponent
        popup = true
    }

    function showImagePopup(index)
    {
        viewMoreImagePopup.author = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.AuthorRole)
        viewMoreImagePopup.authorUrl = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.AuthorUrlRole)
        viewMoreImagePopup.description = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.DescriptionRole)
        viewMoreImagePopup.uuid = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.UuidRole)
        viewMoreImagePopup.thumbnail = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.LargeThumbnailRole)
        viewMoreImagePopup.portrait = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.PortraitUrlRole)
        viewMoreImagePopup.landscape = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.LandscapeUrlRole)
        viewMoreImagePopup.backgroundPortrait = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.BackgroundPortraitRole)
        viewMoreImagePopup.backgroundLandscape = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.BackgroundLandscapeRole)
        viewMoreImagePopup.fullScreen = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.ScreenUrlRole)
        viewMoreImagePopup.portraitSize = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.PortraitSizeRole)
        viewMoreImagePopup.landscapeSize = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.LandscapeSizeRole)
        viewMoreImagePopup.fullScreenSize = imagesModel.data(imagesModel.index(index,0), FeaturedImagesModel.ScreenSizeRole)
        viewMoreImagePopup.opacity = 1
        popupContainer.opacity = 1
        pageArea.opacity = 0
        homePageRoot.popup = true
    }

    function showVideoPopup(index)
    {
        videosModel.setItem(index)
        viewMoreVideoPopup.author = videosModel.data(videosModel.index(index,0), FeaturedVideosModel.AuthorRole)
        viewMoreVideoPopup.authorUrl = videosModel.data(videosModel.index(index,0), FeaturedVideosModel.AuthorUrlRole)
        viewMoreVideoPopup.uuid = videosModel.data(videosModel.index(index,0), FeaturedVideosModel.UuidRole)
        viewMoreVideoPopup.thumbnail = videosModel.data(videosModel.index(index,0), FeaturedVideosModel.ThumbnailRole)
        viewMoreVideoPopup.modelIndex = index
        viewMoreVideoPopup.model = videosModel.itemModel
        viewMoreVideoPopup.opacity = 1
        pageArea.opacity = 0
        popupContainer.opacity = 1
        homePageRoot.popup = true
    }

    function showWallpaperPopup(index)
    {
        viewMoreWallpaperPopup.author = packsModel.data(packsModel.index(index,0), NewestPacksModel.AuthorRole)
        viewMoreWallpaperPopup.authorId = packsModel.data(packsModel.index(index,0), NewestPacksModel.AuthorIdRole)
        viewMoreWallpaperPopup.description = packsModel.data(packsModel.index(index,0), NewestPacksModel.DescriptionRole)
        viewMoreWallpaperPopup.thumbnail = packsModel.data(packsModel.index(index,0), NewestPacksModel.ThumbnailRole)
        viewMoreWallpaperPopup.name = packsModel.data(packsModel.index(index,0), NewestPacksModel.NameRole)
        viewMoreWallpaperPopup.opacity = 1
        popupContainer.opacity = 1
        pageArea.opacity = 0
        homePageRoot.popup = true
    }

    function closePopup()
    {
        if(featuredLoader.item && featuredLoader.item.popup)
        {
            featuredLoader.item.closePopup()
            return
        }

        featuredLoaderItem.opacity = 0
        viewMoreImagePopup.opacity = 0
        viewMoreVideoPopup.opacity = 0
        viewMoreWallpaperPopup.opacity = 0
        popupContainer.opacity = 0
        pageArea.opacity = 1
        homePageRoot.popup = false
    }
}
