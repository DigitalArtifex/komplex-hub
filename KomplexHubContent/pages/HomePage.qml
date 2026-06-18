import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Item {
    readonly property bool searchable: false
    property int resultsPerRow: (homePageRoot.width - (64 + Constants.largeMargin)) / (resultWidth + Constants.largeMargin) + 1
    property int resultWidth: 256
    property int resultHeight: 256

    id: homePageRoot

    NewestPacksModel
    {
        id: newestPacksModel
        resultsPerPage: homePageRoot.resultsPerRow
    }

    FeaturedImagesModel
    {
        id: featuredImagesModel
        resultsPerPage: homePageRoot.resultsPerRow
    }

    FeaturedVideosModel
    {
        id: featuredVideosModel
        resultsPerPage: homePageRoot.resultsPerRow
    }

    Rectangle
    {
        anchors.fill: parent
        color: palette.base

        ScrollView
        {
            id: newPacksView
            anchors.fill: parent
            anchors.margins: Constants.largeMargin

            ColumnLayout
            {
                width: newPacksView.width

                ColumnLayout
                {
                    width: parent.width
                    height: 420

                    Text
                    {
                        color: palette.text
                        font.pixelSize: Constants.h2Font.pixelSize
                        font.bold: true
                        text: qsTr("Newest Wallpaper Packs")
                    }

                    RowLayout
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 256

                        Rectangle
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            color: "transparent"

                            clip: true
                            Row
                            {
                                clip: true
                                spacing: Constants.mediumMargin

                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                anchors.right: newPacksMoreButton.left
                                anchors.leftMargin: Constants.largeMargin
                                anchors.rightMargin: Constants.largeMargin

                                Repeater
                                {
                                    model: newestPacksModel

                                    delegate: Item {
                                        width: resultWidth
                                        height: resultHeight

                                        required property string name
                                        required property string author
                                        required property string description
                                        required property string uuid
                                        required property string thumbnail
                                        required property string authorId

                                        SearchResultItem
                                        {
                                            anchors.fill: parent
                                            title: parent.name
                                            author: parent.author
                                            description: parent.description
                                            thumbnail: parent.thumbnail
                                            uuid: parent.uuid
                                        }
                                    }
                                }
                            }

                            Rectangle
                            {
                                id: newestGradientMap
                                anchors.top: parent.top
                                anchors.right: newPacksMoreButton.left
                                height: resultHeight
                                width: resultWidth / 4

                                gradient: Gradient
                                {
                                    orientation: Gradient.Horizontal
                                    GradientStop { position: 0.0; color: "transparent" }
                                    GradientStop { position: 1.0; color: palette.light }
                                }
                                opacity: 0.5
                            }

                            SquareButton
                            {
                                id: newPacksMoreButton
                                anchors.top: parent.top
                                anchors.right: parent.right
                                height: resultHeight

                                width: 64

                                icon.source: "qrc:/images/icons/icons8-forward.svg"
                                icon.height: 24
                                icon.width: 24
                            }
                        }
                    }
                }

                Item {
                    height: Constants.largeMargin
                }

                ColumnLayout
                {
                    width: parent.width
                    height: 420

                    Text
                    {
                        color: palette.text
                        font.pixelSize: Constants.h2Font.pixelSize
                        font.bold: true
                        text: qsTr("Featured Images")
                    }

                    RowLayout
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 256

                        Rectangle
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            color: "transparent"

                            clip: true
                            Row
                            {
                                clip: true
                                spacing: Constants.mediumMargin

                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                anchors.right: featuredImagesMoreButton.left
                                anchors.leftMargin: Constants.largeMargin
                                anchors.rightMargin: Constants.largeMargin

                                Repeater
                                {
                                    model: featuredImagesModel

                                    delegate: Item
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
                                            author: qsTr("Pexels Images")
                                            description: parent.description
                                            thumbnail: parent.thumbnail
                                            uuid: parent.uuid
                                        }
                                    }
                                }
                            }

                            Rectangle
                            {
                                anchors.top: parent.top
                                anchors.right: featuredImagesMoreButton.left
                                height: resultHeight
                                width: resultWidth / 4

                                gradient: Gradient
                                {
                                    orientation: Gradient.Horizontal
                                    GradientStop { position: 0.0; color: "transparent" }
                                    GradientStop { position: 1.0; color: palette.light }
                                }
                                opacity: 0.5
                            }

                            SquareButton
                            {
                                id: featuredImagesMoreButton
                                anchors.top: parent.top
                                anchors.right: parent.right
                                height: resultHeight

                                width: 64

                                icon.source: "qrc:/images/icons/icons8-forward.svg"
                                icon.height: 24
                                icon.width: 24
                            }
                        }
                    }
                }

                Item {
                    height: Constants.largeMargin
                }

                ColumnLayout
                {
                    width: parent.width
                    height: 420

                    Text
                    {
                        color: palette.text
                        font.pixelSize: Constants.h2Font.pixelSize
                        font.bold: true
                        text: qsTr("Featured Videos")
                    }

                    RowLayout
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 256

                        Rectangle
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            color: "transparent"

                            clip: true
                            Row
                            {
                                clip: true
                                spacing: Constants.mediumMargin

                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                anchors.right: featuredVideosMoreButton.left
                                anchors.leftMargin: Constants.largeMargin
                                anchors.rightMargin: Constants.largeMargin

                                Repeater
                                {
                                    model: featuredVideosModel

                                    delegate: Item {
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
                                            author: parent.author
                                            description: "Video provided by Pexels"
                                            thumbnail: parent.thumbnail
                                            uuid: parent.uuid
                                        }
                                    }
                                }
                            }

                            Rectangle
                            {
                                anchors.top: parent.top
                                anchors.right: featuredVideosMoreButton.left
                                height: resultHeight
                                width: resultWidth / 4

                                gradient: Gradient
                                {
                                    orientation: Gradient.Horizontal
                                    GradientStop { position: 0.0; color: "transparent" }
                                    GradientStop { position: 1.0; color: palette.light }
                                }
                                opacity: 0.5
                            }

                            SquareButton
                            {
                                id: featuredVideosMoreButton
                                anchors.top: parent.top
                                anchors.right: parent.right
                                height: resultHeight

                                width: 64

                                icon.source: "qrc:/images/icons/icons8-forward.svg"
                                icon.height: 24
                                icon.width: 24
                            }
                        }
                    }
                }
            }
        }
    }

    KeroLoadingAnimation
    {
        id: loadingAnimation
        anchors.fill: parent

        OpacityAnimator on opacity {
            duration: 150
        }

        visible: opacity > 0
        opacity: 0
    }

    states: [
        State {
            name: "loading"
            when: newestPacksModel.state == NewestPacksModel.Loading ||
                  featuredImagesModel.state == FeaturedImagesModel.Loading ||
                  featuredVideosModel.state == FeaturedVideosModel.Loading;

            PropertyChanges {
                target: loadingAnimation
                opacity: 1
            }
        }

    ]
}
