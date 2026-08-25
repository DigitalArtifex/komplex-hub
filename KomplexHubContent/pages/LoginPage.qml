/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex
 *  https://digitalartifex.dev - https://github.com/DigitalArtifex
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>
 */
import QtCore
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

    Settings
    {
        id: settings
        property bool animationsEnabled: true
        //ms length of page change
        property int pageChangeAnimationDuration: 250
        property int pageFadeAnimationDuration: 250

        property int slowAnimationDuration: 750
        property int normalAnimationDuration: 250
        property int fastAnimationDuration: 125
    }
}
