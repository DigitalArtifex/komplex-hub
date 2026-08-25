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

Item
{
    property string searchTerm: ""
    property string icon: ""
    property color color: "transparent"

    id: searchBarRootItem
    clip: true

    Rectangle
    {
        color: searchBarRootItem.color
        anchors.fill: parent

        RowLayout
        {
            id: searchContainerLayout
            anchors.fill: parent

            LineEdit
            {
                id: searchEdit
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: Constants.mediumMargin
                Layout.rightMargin: Constants.mediumMargin
                Layout.topMargin: Constants.smallMargin
                Layout.bottomMargin: Constants.smallMargin
                padding.top: 12

                font.pixelSize: 12

                onAccepted: () => searchBarRootItem.searchTerm = text
            }

            SquareButton
            {
                id: searchButton

                Layout.fillHeight: searchEdit.height
                Layout.preferredWidth: searchEdit.height
                Layout.rightMargin: Constants.mediumMargin
                Layout.topMargin: Constants.mediumMargin
                Layout.bottomMargin: Constants.mediumMargin

                icon.source: searchBarRootItem.icon
                icon.width: 16
                icon.height: 16

                onTriggered: () => searchBarRootItem.searchTerm = searchEdit.text
            }
        }
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: searchBarRootItem
                visible: false
            }
        }
    ]

    function reset()
    {
        searchEdit.text = ""
    }
}
