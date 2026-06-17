import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Item {
    readonly property bool searchable: false
    property int resultsPerRow: (homePageRoot.width - (64 + Constants.largeMargin)) / (256 + Constants.largeMargin)

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

    Rectangle {
        anchors.fill: parent
        color: palette.base

        ScrollView {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin

            ColumnLayout {
                width: homePageRoot.width

                // Featured Results
                ColumnLayout {
                    width: parent.width
                    height: 420

                    Text {
                        color: palette.text
                        font.pixelSize: Constants.largeFont.pixelSize
                        text: "Newest Wallpaper Packs"
                    }

                    RowLayout {

                        Layout.fillWidth: true
                        Layout.preferredHeight: 256
                        Layout.alignment: Qt.AlignHCenter

                        Repeater {
                            model: newestPacksModel

                            delegate: Item {
                                width: 256
                                height: 256
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

                        SquareButton {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 64

                            icon.source: "qrc:/images/icons/icons8-forward.svg"
                            icon.height: 24
                            icon.width: 24
                        }
                    }
                }

                Item {
                    height: Constants.largeMargin
                }

                ColumnLayout {
                    width: parent.width
                    height: 420

                    Text {
                        color: palette.text
                        font.pixelSize: Constants.largeFont.pixelSize
                        text: "Featured Images"
                    }

                    RowLayout {

                        Layout.fillWidth: true
                        Layout.preferredHeight: 256
                        Layout.alignment: Qt.AlignHCenter

                        Repeater {
                            model: featuredImagesModel

                            delegate: Item {
                                width: 256
                                height: 256
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

                        SquareButton {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 64

                            icon.source: "qrc:/images/icons/icons8-forward.svg"
                            icon.height: 24
                            icon.width: 24
                        }
                    }
                }

                Item {
                    height: Constants.largeMargin
                }

                ColumnLayout {
                    width: parent.width
                    height: 420

                    Text {
                        color: palette.text
                        font.pixelSize: Constants.largeFont.pixelSize
                        text: "Featured Videos"
                    }

                    RowLayout {

                        Layout.fillWidth: true
                        Layout.preferredHeight: 256
                        Layout.alignment: Qt.AlignHCenter

                        Repeater {
                            model: featuredVideosModel

                            delegate: Item {
                                width: 256
                                height: 256
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

                        SquareButton {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 64

                            icon.source: "qrc:/images/icons/icons8-forward.svg"
                            icon.height: 24
                            icon.width: 24
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
