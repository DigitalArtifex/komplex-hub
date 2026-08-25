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
import KomplexHub.Kero
import KomplexHub.Controls
import KomplexHub.Plugin

Item
{
    property DownloadManager manager: null

    ColumnLayout
    {
        anchors.fill: parent

        VerticalSpacer { }

        KeroWarningAvatar
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 512
            Layout.maximumHeight: 512
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            Layout.maximumHeight: 36
            Layout.margins: Constants.largeMargin

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            SquareButton
            {
                Layout.preferredWidth: 186
                Layout.fillHeight: true
                text: qsTr("Okay")

                onTriggered: () =>
                {
                    if(manager != null)
                    {
                        manager.reset()
                    }
                }
            }
        }

        VerticalSpacer {}
    }
}
