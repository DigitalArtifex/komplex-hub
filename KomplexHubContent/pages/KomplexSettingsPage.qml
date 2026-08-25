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
    property bool updated: updates.length > 0
    property var updates: []

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: Constants.largeMargin

        Text
        {
            Layout.fillWidth: true

            text: qsTr("Animation Settings")
            font: Constants.h1Font
            color: palette.text
        }

        Rectangle
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 320
            Layout.margins: Constants.largeMargin

            color: palette.base
            border.color: palette.midlight

            ColumnLayout
            {
                id: animationSettingsLayout

                anchors.fill: parent
                anchors.margins: Constants.largeMargin

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    CheckBox
                    {
                        Layout.preferredHeight: 36
                        property bool value: settings.animationsEnabled

                        id: animationsEnabledCheckbox
                        text: qsTr("Enable Animations")
                        font: Constants.h3Font
                        checked: value

                        onCheckStateChanged:
                        {
                            if(updates.indexOf("enable"))
                            {
                                updates.splice(updates.indexOf("enable"), 1)
                            }
                            else
                            {
                                updates.push("enable")
                            }

                            value = checked
                        }
                    }
                }
                Rectangle
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    color: "transparent"

                    Rectangle
                    {
                        anchors.centerIn: parent
                        anchors.right: parent.right
                        anchors.left: parent.left
                        height: 2
                        color: palette.midlight
                    }
                }

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    Text
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: qsTr("Slow Animation Duration (Ms)")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Qt.AlignVCenter
                    }

                    TextField
                    {
                        property int value: settings.slowAnimationDuration

                        Layout.fillHeight: true
                        Layout.preferredWidth: 36 * 3

                        id: slowAnimationDurationField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        enabled: animationsEnabledCheckbox.value
                        text: value

                        onEditingFinished: () =>
                        {
                            if(updates.indexOf("slowAnimationDuration"))
                            {
                               updates.splice(updates.indexOf("slowAnimationDuration"), 1)
                            }
                            else
                            {
                               updates.push("slowAnimationDuration")
                            }

                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                slowAnimationDurationField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: slowAnimationDurationField.activeFocus ? palette.base : "transparent"
                            border.color: slowAnimationDurationField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: slowAnimationDurationField
                            anchors.margins: -2
                        }
                    }
                }

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    Text
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: qsTr("Normal Animation Duration (Ms)")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Qt.AlignVCenter
                    }

                    TextField
                    {
                        property int value: settings.normalAnimationDuration

                        Layout.fillHeight: true
                        Layout.preferredWidth: 36 * 3

                        id: normalAnimationDurationField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        enabled: animationsEnabledCheckbox.value
                        text: value

                        onEditingFinished: () =>
                        {
                            if(updates.indexOf("normalAnimationDuration"))
                            {
                                updates.splice(updates.indexOf("normalAnimationDuration"), 1)
                            }
                            else
                            {
                                updates.push("normalAnimationDuration")
                            }

                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                normalAnimationDurationField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: normalAnimationDurationField.activeFocus ? palette.base : "transparent"
                            border.color: normalAnimationDurationField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: normalAnimationDurationField
                            anchors.margins: -2
                        }
                    }
                }

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    Text
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: qsTr("Fast Animation Duration (Ms)")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Qt.AlignVCenter
                    }

                    TextField
                    {
                        property int value: settings.fastAnimationDuration

                        Layout.fillHeight: true
                        Layout.preferredWidth: 36 * 3

                        id: fastAnimationDurationField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        enabled: animationsEnabledCheckbox.value
                        text: value

                        onEditingFinished: () =>
                        {
                            if(updates.indexOf("normalAnimationDuration"))
                            {
                                updates.splice(updates.indexOf("normalAnimationDuration"), 1)
                            }
                            else
                            {
                                updates.push("normalAnimationDuration")
                            }
                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                fastAnimationDurationField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: fastAnimationDurationField.activeFocus ? palette.base : "transparent"
                            border.color: fastAnimationDurationField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: fastAnimationDurationField
                            anchors.margins: -2
                        }
                    }
                }

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    Text
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: qsTr("Page Change Animation Duration (Ms)")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Qt.AlignVCenter
                    }

                    TextField
                    {
                        property int value: settings.pageChangeAnimationDuration

                        Layout.fillHeight: true
                        Layout.preferredWidth: 36 * 3

                        id: pageChangeAnimationDurationField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        enabled: animationsEnabledCheckbox.value
                        text: value

                        onEditingFinished: () =>
                        {
                            if(updates.indexOf("pageChangeAnimationDuration"))
                            {
                                updates.splice(updates.indexOf("pageChangeAnimationDuration"), 1)
                            }
                            else
                            {
                                updates.push("pageChangeAnimationDuration")
                            }
                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                pageChangeAnimationDurationField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: pageChangeAnimationDurationField.activeFocus ? palette.base : "transparent"
                            border.color: pageChangeAnimationDurationField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: pageChangeAnimationDurationField
                            anchors.margins: -2
                        }
                    }
                }

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36

                    Text
                    {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: qsTr("Page Fade Animation Duration (Ms)")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Qt.AlignVCenter
                    }

                    TextField
                    {
                        property int value: settings.pageFadeAnimationDuration

                        Layout.fillHeight: true
                        Layout.preferredWidth: 36 * 3

                        id: pageFadeAnimationDurationField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        text: value
                        enabled: animationsEnabledCheckbox.value

                        onEditingFinished: () =>
                        {
                            if(updates.indexOf("pageChangeAnimationDuration"))
                            {
                                updates.splice(updates.indexOf("pageChangeAnimationDuration"), 1)
                            }
                            else
                            {
                                updates.push("pageChangeAnimationDuration")
                            }
                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                pageFadeAnimationDurationField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: pageFadeAnimationDurationField.activeFocus ? palette.base : "transparent"
                            border.color: pageFadeAnimationDurationField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: pageFadeAnimationDurationField
                            anchors.margins: -2
                        }
                    }
                }
            }
        }

        Item
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
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

    function apply()
    {
        if(updated)
        {
            settings.animationsEnabled = animationsEnabledCheckbox.value
            settings.slowAnimationDuration = slowAnimationDurationField.value
            settings.normalAnimationDuration = normalAnimationDurationField.value
            settings.fastAnimationDuration = fastAnimationDurationField.value
            settings.pageChangeAnimationDuration = pageChangeAnimationDurationField.value
            settings.pageFadeAnimationDuration = pageFadeAnimationDurationField.value

            updates.splice(0, updates.length)
        }
    }

    function reset()
    {
        if(updated)
        {
            animationsEnabledCheckbox.value = settings.animationsEnabled
            slowAnimationDurationField.value = settings.slowAnimationDuration
            normalAnimationDurationField.value = settings.normalAnimationDuration
            fastAnimationDurationField.value = settings.fastAnimationDuration
            pageChangeAnimationDurationField.value = settings.pageChangeAnimationDuration
            pageFadeAnimationDurationField.value = settings.pageFadeAnimationDuration

            updates.splice(0, updates.length)
        }
    }
}
