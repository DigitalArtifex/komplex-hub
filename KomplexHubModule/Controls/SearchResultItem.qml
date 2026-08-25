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
    property string author: ""
    property string description: ""
    property string title: ""
    property string thumbnail: ""
    property string uuid: ""
    property bool selected: false
    property bool pexels: false

    signal triggered
    signal viewMoreTriggered

    id: rootItem
    width: 256
    height: 256
    clip: true

    Rectangle
    {
        id: searchResultItemContainer
        anchors.fill: parent
        border.color: palette.alternateBase.lighter(1.75)
        color: palette.alternateBase.lighter(1.25)

        MouseArea
        {
            property string lastState
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            onEntered: () => {
                states.hovered = true
            }

            onExited: () => {
                if(!viewMoreButton.hovered)
                    states.hovered = false
            }

            onReleased: () => {
                if(!states.selected)
                {
                    rootItem.triggered()
                }
            }
        }

        ColumnLayout
        {
            anchors.fill: parent

            ImageFrame
            {
                Layout.preferredWidth: 250
                Layout.preferredHeight: 141
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.topMargin: 3
                source: rootItem.thumbnail

                Image
                {
                    antialiasing: true
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: Constants.mediumMargin

                    width: 24
                    height: 24
                    fillMode: Image.PreserveAspectFit
                    opacity: 0.5
                    source: "qrc:/images/icons/pexels-icon-filled-256.svg"

                    visible: rootItem.pexels
                }
            }

            Text
            {
                color: palette.text
                text: rootItem.title

                font.pixelSize: 16
                font.bold: true
                elide: Text.ElideRight

                Layout.preferredWidth: 246
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            Item { Layout.fillHeight: true }

            Text
            {
                id: descriptionText
                clip: true
                color: palette.text
                font.pixelSize: 12
                text: rootItem.description
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                textFormat: Text.PlainText
                elide: Text.ElideRight

                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Layout.maximumHeight: 32
                Layout.preferredHeight: 32
                Layout.preferredWidth: 236
                Layout.maximumWidth: 236
                Layout.bottomMargin: 3
            }

            Text
            {
                color: palette.text
                font.pixelSize: 12
                text: "By: " + rootItem.author
                leftPadding: Constants.mediumMargin
                rightPadding: Constants.mediumMargin
                horizontalAlignment: Qt.AlignRight

                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                Layout.fillWidth: true
            }

            SquareButton
            {
                id: viewMoreButton
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Layout.maximumHeight: 50
                Layout.preferredHeight: 32
                Layout.preferredWidth: 236
                Layout.maximumWidth: 236
                Layout.bottomMargin: 3

                text: qsTr("View More")
                opacity: 0

                onTriggered: () => {
                    rootItem.viewMoreTriggered()
                }
            }

            Item { Layout.fillHeight: true }
        }
    }

    QtObject
    {
        id: states
        property bool clicked: false
        property bool hovered: false
        property alias selected: rootItem.selected
        property bool idle: true

        onIdleChanged: () =>
        {
            if(idle)
            {
                clicked = false
                hovered = false
                selected = false
            }
        }
    }

    states: [
        State {
            name: "hovered"
            when: states.hovered && !states.selected

            PropertyChanges {
                target: searchResultItemContainer
                color: palette.alternateBase.lighter(1.5)
                border.color: Qt.hsva(palette.accent.hslHue,
                                      palette.accent.hslSaturation,
                                      0.5, // Set custom lightness
                                      1.0)
            }

            PropertyChanges
            {
                target: viewMoreButton
                opacity: 0

                Layout.preferredHeight: 0
            }

            PropertyChanges
            {
                target: descriptionText
                opacity: 1

                Layout.preferredHeight: 32
            }
        },
        State {
            name: "selected"
            when: states.selected && !states.hovered

            PropertyChanges
            {
                target: searchResultItemContainer
                color: palette.accent
            }
        },
        State
        {
            name: "view_more"
            when: states.selected && states.hovered

            PropertyChanges
            {
                target: viewMoreButton
                opacity: 1

                Layout.preferredHeight: 32
            }

            PropertyChanges
            {
                target: descriptionText
                opacity: 0

                Layout.preferredHeight: 0
            }
        },

        State
        {
            name: "default"
            when: states.idle

            PropertyChanges
            {
                target: viewMoreButton
                opacity: 0

                Layout.preferredHeight: 0
            }

            PropertyChanges
            {
                target: descriptionText
                opacity: 1

                Layout.preferredHeight: 32
            }
        }

    ]

    transitions:
    [
        Transition
        {
            to: "selected"

            ColorAnimation
            {
                target: searchResultItemContainer
                duration: 250
            }
        },

        Transition
        {
            from: "selected"
            to: "view_more"

            NumberAnimation
            {
                target: viewMoreButton
                property: "opacity"
                duration: 250
                easing.type: Easing.InOutQuad
            }
        },

        Transition
        {
            from: "view_more"
            to: "selected"

            NumberAnimation
            {
                target: descriptionText
                property: "opacity"
                duration: 250
                easing.type: Easing.InOutQuad
            }
        },

        Transition
        {
            to: "default"

            ColorAnimation
            {
                target: searchResultItemContainer
                duration: 250
            }
        },

        Transition
        {
            to: "hovered"

            ColorAnimation
            {
                target: searchResultItemContainer
                duration: 250
            }
        }
    ]
}
