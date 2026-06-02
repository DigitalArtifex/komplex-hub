import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero

Item {
    readonly property bool searchable: false
    property int resultsPerRow: (homePageRoot.width - (64 + Constants.largeMargin)) / (256 + Constants.largeMargin)

    id: homePageRoot

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
                        text: "Featured"
                    }

                    RowLayout {

                        Layout.fillWidth: true
                        Layout.preferredHeight: 256
                        Layout.alignment: Qt.AlignHCenter

                        Repeater {
                            model: resultsPerRow

                            delegate: SearchResultItem {

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
                        text: "Newest"
                    }

                    RowLayout {

                        Layout.fillWidth: true
                        Layout.preferredHeight: 256
                        Layout.alignment: Qt.AlignHCenter

                        Repeater {
                            model: resultsPerRow

                            delegate: SearchResultItem {

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
                        text: "Popular"
                    }

                    RowLayout {

                        Layout.fillWidth: true
                        Layout.preferredHeight: 256
                        Layout.alignment: Qt.AlignHCenter

                        Repeater {
                            model: resultsPerRow

                            delegate: SearchResultItem {

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
}
