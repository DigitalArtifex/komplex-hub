/*
 *  QML LineEdit Control
 *  Copyright (C) 2026 @DigitalArtifex | github.com/DigitalArtifex | digitalartifex.dev
 *
 *  This control provides a wrapped TextInput that looks and acts more like the
 *  LineEdit control from QtWidgets
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

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {

    /* Grouped Settings */
    component BorderSettings: QtObject {
        property color color: palette.alternateBase.lighter()
        property int width: 1
        property bool pixelAligned: true
    }

    component Corners: QtObject {
        property int bottom: 0
        property int left: 0
        property int right: 0
        property int top: 0
    }

    component Radius: QtObject {
        property int topLeft: 0
        property int topRight: 0
        property int bottomLeft: 0
        property int bottomRight: 0
    }
    /* End Grouped Settings */

    property var background: palette.alternateBase.darker()
    property BorderSettings border: BorderSettings {}

    property Corners margins: Corners {bottom: 3; top: 3; left: 3; right: 3}
    property Radius radius: Radius {}
    property font font
    property color color: palette.text
    property Corners padding: Corners {bottom: 6; top: 6; left: 6; right: 6}
    property alias text: lineEditInput.text

    signal accepted
    signal editingFinished

    id: lineEditRoot

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            id: lineEditBackground

            border.color: lineEditRoot.border.color
            border.width: lineEditRoot.border.width
            border.pixelAligned: lineEditRoot.border.pixelAligned

            color: lineEditRoot.background

            Layout.fillHeight: true
            Layout.fillWidth: true

            Layout.topMargin: lineEditRoot.margins.top
            Layout.bottomMargin: lineEditRoot.margins.bottom
            Layout.leftMargin: lineEditRoot.margins.left
            Layout.rightMargin: lineEditRoot.margins.right

            bottomLeftRadius: lineEditRoot.radius.bottomLeft
            bottomRightRadius: lineEditRoot.radius.bottomRight
            topLeftRadius: lineEditRoot.radius.topLeft
            topRightRadius: lineEditRoot.radius.topRight

            TextInput {
                id: lineEditInput
                anchors.fill: parent
                color: lineEditRoot.color
                font: lineEditRoot.font

                anchors.leftMargin: lineEditRoot.padding.left
                anchors.topMargin: lineEditRoot.padding.top
                anchors.bottomMargin: lineEditRoot.padding.bottom
                anchors.rightMargin: lineEditRoot.padding.right

                onAccepted: () => lineEditRoot.accepted()
                onTextChanged: () => lineEditRoot.text = lineEditInput.text
                onEditingFinished: () => lineEditRoot.editingFinished()
            }

            states: [
                State {
                    name: "focused"
                    when: lineEditInput.focus
                    PropertyChanges {
                        target: lineEditBackground
                        border.color: palette.accent
                    }
                }

            ]
        }
    }
}
