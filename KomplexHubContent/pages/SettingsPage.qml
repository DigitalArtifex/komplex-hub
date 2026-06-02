import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub
import KomplexHub.Kero

Item {
    readonly property bool searchable: false

    id: settingsPageRoot

    Rectangle {
        anchors.fill: parent
        color: palette.base

        ColumnLayout {
            anchors.fill: parent

            TabBar {
                id: bar

                Layout.fillHeight: false
                Layout.fillWidth: true
                TabButton {
                    text: qsTr("Wallpaper")
                }
                TabButton {
                    text: qsTr("Account")
                }
                TabButton {
                    text: qsTr("About")
                }
            }

            StackLayout {

                Layout.fillHeight: true
                Layout.fillWidth: true
                currentIndex: bar.currentIndex

                Item {
                    id: wallpaperTab

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    WallpaperSettingsPage {
                        anchors.fill: parent
                    }
                }
                Item {
                    id: accountTab

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    UserProfilePage {
                        anchors.fill: parent
                    }
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    property string title: qsTr("Hello!")
                    property string contributors: qsTr("DigitalArtifex

fridje")
                    property string thanks: qsTr("Icons8

ShaderToy

Pexels")

                    id: keroHelloAvatarRoot

                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"

                        ColumnLayout {
                            anchors.fill: parent

                            KeroHelloAvatar {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                antialiasing: true
                            }

                            Item { Layout.preferredHeight: 36 }

                            Text {
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.margins: Constants.largeMargin
                                text: qsTr("Meet Kero, the small but tireless spirit behind Komplex, a curious little dragon whose name echoes the cheerful sound of a frog’s call. Hailing from the realm of **Kairoku**, Kero honed his skill in a place where unfinished ideas drift like clouds and raw creativity hums in the air.")
                                color: palette.text

                                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                                Layout.bottomMargin: Constants.largeMargin
                                wrapMode: Text.WordWrap
                                textFormat: Text.MarkdownText
                            }

                            Text {
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.margins: Constants.largeMargin
                                text: qsTr("In Kairoku, landscapes aren’t painted...they’re assembled, animated, and brought to life through careful craftsmanship. Kero was once an apprentice there, learning how to shape reactive skies, breathing forests, and shimmering cityscapes. With his trusty tools and a builder’s instinct, he became known for turning even the simplest concept into something that felt alive.")
                                color: palette.text

                                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                                Layout.bottomMargin: Constants.largeMargin
                                wrapMode: Text.WordWrap
                                textFormat: Text.MarkdownText
                            }

                            Item { Layout.fillHeight: true }

                            RowLayout {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignTop | Qt.AlignRight

                                ColumnLayout {
                                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                                    Layout.margins: Constants.largeMargin
                                    Layout.fillHeight: true

                                    Text {
                                        text: qsTr("Contributors")
                                        color: palette.text.darker()
                                        font: Constants.h5Font

                                        Layout.alignment: Qt.AlignTop
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }

                                    Text {
                                        id: contributorText
                                        text: keroHelloAvatarRoot.contributors
                                        color: palette.text.darker()

                                        Layout.alignment: Qt.AlignBottom
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }
                                }

                                ColumnLayout {
                                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                                    Layout.margins: Constants.largeMargin
                                    Layout.fillHeight: true

                                    Text {
                                        text: qsTr("Special Thanks")
                                        color: palette.text.darker()
                                        font: Constants.h5Font

                                        Layout.alignment: Qt.AlignTop
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }

                                    Text {
                                        id: thanksText
                                        text: keroHelloAvatarRoot.thanks
                                        color: palette.text.darker()

                                        Layout.alignment: Qt.AlignBottom
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }

    }
}
