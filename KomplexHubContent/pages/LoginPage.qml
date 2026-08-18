import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin
import KomplexHub.Views
import KomplexHub.Pages

Item {
    id: loginFormRoot

    ColumnLayout {
        anchors.centerIn: parent

        Image {
            source: "qrc:/images/icons/icons8-user-account.svg"
            Layout.preferredHeight: 64
            Layout.preferredWidth: 64
            Layout.alignment: Qt.AlignCenter
        }

        // KeroLoginAvatar {
        //     Layout.preferredWidth: 380
        //     Layout.preferredHeight: 380
        //     description: ""
        //     Layout.alignment: Qt.AlignCenter
        // }

        // Image {
        //     source: "images/kero/kero_choice.png"
        //     Layout.preferredWidth: 380
        //     Layout.preferredHeight: 380
        //     Layout.alignment: Qt.AlignCenter
        // }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            ColumnLayout {
                Layout.fillWidth: true

                Text {
                    Layout.fillWidth: false
                    text: "Username"

                    font.pixelSize: Constants.largeFont.pixelSize
                }
                LineEdit {
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 256
                }
            }

            ColumnLayout {
                Layout.fillWidth: true

                Text {
                    Layout.fillWidth: false
                    text: "Password"

                    font.pixelSize: Constants.largeFont.pixelSize
                }
                LineEdit {
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 256
                }
            }

            RowLayout {
                clip: true
                Layout.maximumWidth: 250
                Layout.leftMargin: 3

                SquareButton {
                    Layout.preferredHeight: 36
                    Layout.preferredWidth: 36

                    icon.source: "qrc:/images/icons/icons8-cancel.svg"
                }

                SquareButton {
                    Layout.fillWidth: true
                    icon.source: "qrc:/images/icons/icons8-login.svg"
                    text: qsTr("Login")
                }
            }
        }
    }
}
