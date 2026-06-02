import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    property string title: qsTr("No Results")
    property string description: qsTr("Kero couldn't find any wallpapers with that search term")
    property color color: palette.window

    id: noResultsOverlayRoot

    Rectangle {
        id: noResultsOverlayBackground
        anchors.fill: parent
        color: noResultsOverlayRoot.color

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 64
                Layout.minimumHeight: 64
                Layout.maximumWidth: 512
                Layout.maximumHeight: 512
                Layout.alignment: Qt.AlignCenter

                color: "transparent"

                Image {
                    anchors.fill: parent

                    source: "qrc:/images/kero/kero_no_results.png"
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit
                }

                ColumnLayout {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Text {
                        color: palette.text
                        text: noResultsOverlayRoot.title
                        font.pixelSize: Constants.largeFont.pixelSize * 1.5

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    }
                    Text {
                        id: errorText
                        text: noResultsOverlayRoot.description
                        color: palette.text.darker()

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.bottomMargin: Constants.largeMargin
                    }
                }
            }
        }
    }
}
