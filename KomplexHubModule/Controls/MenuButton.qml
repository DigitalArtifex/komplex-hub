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

Item {
    id: menuButtonRoot
    property string text: "Button Text"
    property IconSettings icon: IconSettings {}
    property var page: null
    property bool selected: false

    signal triggered

    component IconSettings: QtObject {
        property int width: 16
        property int height: 16
        property string url: ""
    }

    Rectangle {
        id: menuButtonBackground

        anchors.fill: parent
        color: "transparent"

        ColumnLayout {
            spacing: 0
            anchors.fill: parent
            layoutDirection: Qt.LeftToRight

            Image {
                id: menuButtonIcon
                source: icon.url

                Layout.preferredWidth: icon.height
                Layout.preferredHeight: icon.width
                Layout.topMargin: Constants.mediumMargin
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            Text {
                color: palette.text
                text: menuButtonRoot.text

                elide: Text.ElideMiddle
                horizontalAlignment: Qt.AlignHCenter
                font.pixelSize: 12

                Layout.fillWidth: true
                Layout.topMargin: Constants.mediumMargin
                Layout.bottomMargin: Constants.mediumMargin
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }
        }

        Behavior on color {
            ColorAnimation { duration: Constants.fastAnimationDuration }
        }
    }

    MouseArea {
        property string lastState
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true

        onEntered: () => {
            menuButtonRoot.state = "hovered"
        }

        onExited: () => {
            lastState = ""

            if(selected)
                menuButtonRoot.state = "selected"
            else
                menuButtonRoot.state = ""
        }

        onPressed: () => {
            lastState = menuButtonRoot.state
            menuButtonRoot.state = "clicked"
        }

        onReleased: () => {
            if(!mouseArea.containsMouse)
                return;

            if(!selected)
                selected = true
        }
    }

    onSelectedChanged: () => {
        if(selected) {
            triggered()
            state = "selected"
        }
        else {
            state = ""
        }
    }

    states: [
        State {
            name: "selected"

            PropertyChanges {
                target: menuButtonBackground
                color: palette.accent.darker(1.5)
            }
        },
        State {
            name: "hovered"
            //when: mouseArea.containsMouse

            PropertyChanges {
                target: menuButtonBackground
                color: palette.button
            }
        },
        State {
            name: "clicked"
            //when: mouseArea.containsPress

            PropertyChanges {
                target: menuButtonBackground
                color: palette.button.darker(1.5)
            }
        }
    ]
}
