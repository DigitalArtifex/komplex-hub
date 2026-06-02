import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    property string title: qsTr("Warning")
    property string description: qsTr("Kero noticed there might be an issue")
    property color color: palette.window

    id: keroWarningAvatarRoot

    Rectangle {
        id: keroWarningAvatarBackground
        anchors.fill: parent
        color: keroWarningAvatarRoot.color

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

                    source: "../images/kero/kero_warning.png"
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit
                }

                ColumnLayout {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Text {
                        color: palette.text
                        text: keroWarningAvatarRoot.title
                        font.pixelSize: Constants.largeFont.pixelSize * 1.5

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    }

                    Text {
                        id: errorText
                        text: keroWarningAvatarRoot.description
                        color: palette.text.darker()

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.bottomMargin: Constants.largeMargin
                    }
                }
            }
        }
    }
}
