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
    id: root

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            Image {
                Layout.preferredHeight: 128
                Layout.preferredWidth: 128
                id: userAccountImage
                source: "qrc:/images/icons/icons8-user-account.svg"
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: Constants.largeMargin
                Layout.leftMargin: Constants.largeMargin
                Layout.bottomMargin: Constants.largeMargin
                Layout.rightMargin: Constants.largeMargin
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text {
                    text: qsTr("Username")
                    font.pixelSize: Constants.largeFont.pixelSize
                    font.bold: true
                    color: palette.text
                }

                Text {
                    text: qsTr("Thursday, April 30th 2026")
                    font.pixelSize: Constants.mediumMargin.pixelSize
                    color: palette.text
                }

                RowLayout {
                    Text {
                        text: qsTr("Wallpapers Submitted:")
                        font.pixelSize: Constants.largeFont.pixelSize
                        color: palette.text
                    }
                    Text {
                        text: qsTr("12")
                        font.pixelSize: Constants.largeFont.pixelSize
                        font.bold: true
                        color: palette.text
                    }
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
