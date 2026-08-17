import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub

Item {
    property string title: qsTr("Success!")
    property string description: qsTr("Your new wallpaper is ready!")
    property color color: "transparent"

    id: keroHelloAvatarRoot

    Rectangle {
        id: keroHelloAvatarBackground
        anchors.fill: parent
        color: keroHelloAvatarRoot.color

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

                    source: "qrc:/images/kero/kero_success.png"
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit
                }

                ColumnLayout {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Text {
                        color: palette.text
                        text: keroHelloAvatarRoot.title
                        font: Constants.h1Font

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    }

                    Text {
                        id: errorText
                        text: keroHelloAvatarRoot.description
                        color: palette.text.darker()

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.bottomMargin: Constants.largeMargin
                    }
                }
            }
        }
    }
}
