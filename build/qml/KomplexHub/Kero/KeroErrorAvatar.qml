import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    property string title: qsTr("Error")
    property string description: qsTr("A description of the error")
    property color color: palette.window

    id: errorOverlayRoot

    Rectangle {
        id: errorOverlayBackground
        anchors.fill: parent
        color: errorOverlayRoot.color

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

                    source: "../images/kero/kero_error.png"
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit
                }

                ColumnLayout {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Text {
                        color: palette.text
                        text: errorOverlayRoot.title
                        font: Constants.h1Font

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    }
                    Text {
                        id: errorText
                        text: errorOverlayRoot.description
                        color: palette.text.darker()

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.bottomMargin: Constants.largeMargin
                    }
                }
            }
        }
    }
}
