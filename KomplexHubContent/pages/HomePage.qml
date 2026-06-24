import QtQuick 2.15
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

                        selected: parent.highlighted
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: () => packsPaginator.currentIndex = index
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

                    SearchResultItem
                    {
                        anchors.fill: parent
                        title: parent.author
                        description: parent.description
                        thumbnail: parent.thumbnail
                        uuid: parent.uuid
                        pexels: true
                        selected: parent.highlighted
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: () => imagesPaginator.currentIndex = index
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

                    SearchResultItem
                    {
                        anchors.fill: parent
                        title: parent.author
                        thumbnail: parent.thumbnail
                        uuid: parent.uuid
                        pexels: true
                        selected: parent.highlighted
                    }

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: () => videosPaginator.currentIndex = index
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
                target: loadingOverlay
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
}
