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
import QtQuick.Layouts
import QtQuick.Controls

import KomplexHub

Item
{
    id: itemRoot

    property int page: 0
    property int totalPages: 0

    property bool enablePrevious: false
    property bool enableNext: false

    signal next
    signal previous

    RowLayout
    {
        id: buttonBoxLayout
        anchors.fill: parent

        SquareButton
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 128

            icon.source: "qrc:/images/icons/icons8-back.svg"
            icon.height: 16
            icon.width: 16
            text: qsTr("Previous")
            enabled: itemRoot.enablePrevious

            onTriggered: () => itemRoot.previous()
        }

        Item { Layout.fillWidth: true }

        SquareButton
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 128

            icon.source: "qrc:/images/icons/icons8-next.svg"
            icon.height: 16
            icon.width: 16
            text: qsTr("Next")
            enabled: itemRoot.enableNext

            onTriggered: () => itemRoot.next()
        }
    }
}
