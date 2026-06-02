/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2025 @DigitalArtifex | github.com/DigitalArtifex
 *
 *  config.qml
 *
 *  This component provides a configuration interface for the Komplex Wallpaper Engine,
 *  allowing users to customize shader settings, channel configurations, and other
 *  parameters related to the wallpaper engine.
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
// pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
//import org.kde.kirigami as Kirigami
//import org.kde.plasma.core as PlasmaCore
import QtCore
import Qt.labs.folderlistmodel 2.15
import KomplexHub
import KomplexHub.Controls
import KomplexHubPlugin

//import com.github.digitalartifex.komplex 1.0 as Komplex

Item
{
    SettingsManager {
        id: manager

        onError: (title, message) => {
            console.log(title + ": " + message)
        }
    }

    id: root
    property alias cfg_pauseMode: pauseModeCombo.currentIndex
    property alias cfg_isPaused: runningCombo.checked
    property alias cfg_shaderSpeed: speedSlider.value
    property alias cfg_mouseSpeedBias: mouseBiasSlider.value
    property bool cfg_infoPlasma6Preview_dismissed
    property bool cfg_warningResources_dismissed
    property bool cfg_emergencyHelp_dismissed
    property bool cfg_infoiChannelSettings_dismissed
    property alias cfg_checkActiveScreen: activeScreenOnlyCheckbox.checked
    property alias cfg_excludeWindows: excludeWindows.windows
    property alias cfg_running: runningCombo.checked

    property alias cfg_resolution_x: resolutionXField.value
    property alias cfg_resolution_y: resolutionYField.value

    property alias cfg_framerate_limit: frameRateField.value
    property bool cfg_shader_updated

    property string shaderPackage: ""
    property int shaderPackageIndex: 0
    property real shaderSpeed: 1.0
    property int resolutionX: 1080
    property int resolution: 1920
    property bool enableMouseTracking: true
    property real mouseTrackingBias: 0.5

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        clip: true

        ScrollView {
            id: wallpaperSettingsScrollView
            anchors.fill: parent
            anchors.margins: Constants.largeMargin

            ColumnLayout {
                width: wallpaperSettingsScrollView.width

                Text {
                    text: qsTr("Wallpaper Options")
                    font: Constants.h2Font
                    color: palette.text
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                ComboBox
                {
                    property string shader
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                    Layout.margins: Constants.smallMargin

                    id: selectedShaderPack
                    model: manager.availableShaderPacks

                    delegate: Component
                    {
                        id: packListDelegate

                        ItemDelegate
                        {
                            width: parent ? parent.width : 0
                            text: modelData
                        }
                    }

                    textRole: "modelData"
                    currentIndex: manager.shaderPackIndex
                    displayText: currentIndex === -1 ? "Custom File" : currentText.replace("_", " ").charAt(0).toUpperCase() + currentText.replace("_", " ").slice(1)

                    // onCurrentTextChanged:
                    // {
                    //     root.cfg_shader_package_index = currentIndex;

                    //     if (root.cfg_shader_package_index === -1)
                    //         return;

                    //     var source = currentText
                    //     shaderPackModel.loadMetadata(source)
                    //     root.cfg_shader_package = "file://" + shaderPackModel.path(source)
                    // }


                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: Constants.smallMargin

                    Text {
                        text: qsTr("Render Resolution")
                        font: Constants.h3Font
                        color: palette.text
                        Layout.preferredHeight: 36
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        text: qsTr("X")
                        font: Constants.h3Font
                        color: palette.text
                        Layout.preferredHeight: 36
                        Layout.preferredWidth: 64
                        verticalAlignment: Text.AlignVCenter
                    }

                    TextField {
                        property int value: manager.resolutionX

                        Layout.preferredHeight: 36
                        Layout.preferredWidth: 36 * 3

                        id: resolutionXField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        text: value
                        onEditingFinished: () =>
                        {
                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                resolutionXField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: resolutionXField.activeFocus ? palette.base : "transparent"
                            border.color: resolutionXField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: resolutionXField
                            anchors.margins: -2
                        }
                    }

                    Text {
                        text: qsTr("Y")
                        font: Constants.h3Font
                        color: palette.text
                        Layout.preferredHeight: 36
                        Layout.preferredWidth: 64
                        verticalAlignment: Text.AlignVCenter
                    }

                    TextField {
                        property int value: manager.resolutionY

                        Layout.preferredHeight: 36
                        Layout.preferredWidth: 36 * 3

                        id: resolutionYField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        text: value

                        onEditingFinished: () =>
                        {
                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                resolutionYField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: resolutionYField.activeFocus ? palette.base : "transparent"
                            border.color: resolutionYField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: resolutionYField
                            anchors.margins: -2
                        }
                    }
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout
                {
                    Layout.fillWidth: true

                    Text {
                        text: qsTr("Frame Rate")
                        font: Constants.h3Font
                        color: palette.text
                        Layout.preferredHeight: 36
                    }

                    Slider
                    {
                        id: frameRateSlider
                        Layout.fillWidth: true
                        Layout.preferredHeight: 36
                        from: 1
                        to: 60
                        stepSize: 0.01
                        onValueChanged: frameRateField.text = String(value.toFixed(2));

                        Component.onCompleted: () => {
                            value = manager.targetFramerate
                        }
                    }

                    TextField
                    {
                        property int value

                        id: frameRateField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        Layout.preferredWidth: 36 * 3
                        Layout.preferredHeight: 36

                        onEditingFinished: () =>
                        {
                            value = parseInt(text)
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                frameRateField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: frameRateField.activeFocus ? palette.base : "transparent"
                            border.color: frameRateField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: frameRateField
                            anchors.margins: -2
                        }
                    }
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout
                {
                    id: speedLayout
                    Layout.fillWidth: true

                    Text {
                        text: qsTr("Shader Speed")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredHeight: 36
                    }

                    Slider
                    {
                        id: speedSlider
                        Layout.preferredHeight: 36
                        Layout.fillWidth: true
                        from: -4.0
                        to: 4.0
                        stepSize: 0.01
                        onValueChanged: shaderSpeedField.text = String(value.toFixed(2));

                        Component.onCompleted: () => {
                            value = manager.shaderSpeed
                        }
                    }

                    TextField
                    {
                        id: shaderSpeedField
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        Layout.preferredWidth: 36 * 3
                        Layout.preferredHeight: 36

                        onEditingFinished: () =>
                        {
                            let inputValue = parseFloat(text);

                            if (isNaN(inputValue) || inputValue < speedSlider.from)
                                inputValue = speedSlider.from;
                            else if (inputValue > speedSlider.to)
                                inputValue = speedSlider.to;

                            text = inputValue.toFixed(2);
                            speedSlider.value = inputValue;
                        }

                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                shaderSpeedField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }

                        background: Rectangle
                        {
                            color: shaderSpeedField.activeFocus ? palette.base : "transparent"
                            border.color: shaderSpeedField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: shaderSpeedField
                            anchors.margins: -2
                        }
                    }

                }

                Item { Layout.preferredHeight: Constants.largeMargin }

                Text {
                    text: qsTr("Run Options")
                    font: Constants.h2Font
                    color: palette.text
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout {
                    Layout.fillWidth: true

                    Text {
                        text: qsTr("Pause Mode")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Text.AlignVCenter
                    }

                    ComboBox
                    {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 36

                        id: pauseModeCombo

                        model: [
                                qsTr("Maximized or full-screen windows"),
                                qsTr("Active window is present"),
                                qsTr("At least one window is shown"),
                                qsTr("Never")
                        ]

                        textRole: modelData
                        //onCurrentIndexChanged: root.cfg_pauseMode = currentIndex
                        currentIndex: manager.pauseMode
                    }
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                CheckBox
                {
                    id: activeScreenOnlyCheckbox
                    checked: manager.onlyCheckActiveScreen
                    text: qsTr("Only check for windows in active screen")
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout
                {
                    Layout.fillWidth: true

                    Text {
                        text: qsTr("Excluded Windows")
                        font: Constants.h3Font
                        color: palette.text
                    }

                    TextField
                    {
                        id: excludeWindows
                        property var windows: manager.excludedWindows
                        Layout.fillWidth: true
                        Layout.preferredHeight: 36

                        text: windows.join(",")
                        onEditingFinished: () =>
                        {
                            windows = excludeWindows.text.trim().replace(/\s+/g, "").split(",");
                        }

                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("A comma-separated list of fully-qualified App-IDs to exclude their windows from triggering pause mode.")
                    }
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                CheckBox
                {
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", cfg_isPaused ? "Playing" : "Paused")

                    id: runningCombo
                    checked: manager.running
                    text: qsTr("Shader Is Running")
                }

                Item { Layout.preferredHeight: Constants.largeMargin }

                Text {
                    text: qsTr("Mouse Tracking")
                    font: Constants.h2Font
                    color: palette.text
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout
                {
                    Layout.fillWidth: true

                    Text {
                        text: qsTr("Mouse Tracking Bias")
                        font: Constants.h3Font
                        color: palette.text
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredHeight: 36
                    }
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", "Mouse bias:")
                    Slider
                    {
                        id: mouseBiasSlider
                        Layout.preferredHeight: 36
                        Layout.fillWidth: true
                        from: 0.0
                        to: 4.0
                        stepSize: 0.01
                        value: manager.mouseTrackingBias
                        onValueChanged: () =>
                        {
                            mouseBiasField.text = String(value.toFixed(2));
                        }
                    }
                    TextField
                    {
                        id: mouseBiasField
                        text: manager.mouseTrackingBias
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        horizontalAlignment: Text.AlignRight
                        Layout.preferredWidth: 36 * 3
                        Layout.preferredHeight: 36
                        onEditingFinished: () =>
                        {
                            let inputValue = parseFloat(text);

                            if (isNaN(inputValue) || inputValue < mouseBiasSlider.from)
                                inputValue = mouseBiasSlider.from;
                            else if (inputValue > mouseBiasSlider.to)
                                inputValue = mouseBiasSlider.to;

                            text = inputValue.toFixed(2);
                            mouseBiasSlider.value = inputValue;
                        }
                        Keys.onPressed: (event) =>
                        {
                            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                            {
                                mouseBiasField.focus = false; // Unfocus the TextField
                                event.accepted = true; // Prevent further propagation of the key event
                            }
                        }
                        background: Rectangle
                        {
                            color: mouseBiasField.activeFocus ? palette.base : "transparent"
                            border.color: mouseBiasField.activeFocus ? palette.highlight : "transparent"
                            border.width: 1
                            radius: 4
                            anchors.fill: mouseBiasField
                            anchors.margins: -2
                        }
                    }
                }

                Item { Layout.preferredHeight: Constants.smallMargin }

                CheckBox
                {
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", cfg_isPaused ? "Playing" : "Paused")

                    id: mouseTrackingCheckbox
                    checked: manager.mouseTrackingEnabled
                    text: qsTr("Mouse Tracking Enabled")
                }

                // Button
                // {
                //     visible: navBar.currentIndex === 2
                //     id: kofiButton
                //     Layout.preferredWidth: Kirigami.Units.gridUnit * 5
                //     Layout.preferredHeight: Kirigami.Units.gridUnit * 3

                //     contentItem: RowLayout
                //     {
                //         AnimatedImage
                //         {
                //             source: "icons/kofi.gif"
                //             sourceSize.width: 36
                //             sourceSize.height: 36
                //             fillMode: Image.Pad
                //             horizontalAlignment: Image.AlignLeft
                //             transform: Translate
                //             {
                //                 x: 8
                //             }
                //         }
                //         Text
                //         {
                //             text: i18nd("@button:kofi", "Kofi")
                //             horizontalAlignment: Text.AlignHCenter
                //             color: palette.text
                //             transform: Translate
                //             {
                //                 x: -8
                //             }
                //         }
                //     }
                //     onClicked: () =>
                //     {
                //         Qt.openUrlExternally("https://ko-fi.com/digitalartifex");
                //     }
                // }

                Item { Layout.preferredHeight: Constants.smallMargin }

                RowLayout
                {
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", "Author:")
                    Text
                    {
                        Layout.preferredWidth: Kirigami.Units.gridUnit * 11
                        text: shaderPackModel.metadata.author
                        horizontalAlignment: Text.AlignLeft
                        color: palette.text
                    }
                }

                RowLayout
                {
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", "Description:")
                    Text
                    {
                        Layout.preferredWidth: Kirigami.Units.gridUnit * 16
                        text: shaderPackModel.metadata.description
                        horizontalAlignment: Text.AlignLeft
                        color: palette.text
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        elide: Text.ElideRight
                    }
                }

                RowLayout
                {
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", "License:")
                    Text
                    {
                        Layout.preferredWidth: Kirigami.Units.gridUnit * 11
                        text: shaderPackModel.metadata.license
                        horizontalAlignment: Text.AlignLeft
                        color: palette.text
                    }
                }

                RowLayout
                {
                    //Kirigami.FormData.label: i18nd("com.github.digitalartifex.komplex", "Version:")
                    Text
                    {
                        Layout.preferredWidth: Kirigami.Units.gridUnit * 11
                        text: shaderPackModel.metadata.version
                        horizontalAlignment: Text.AlignLeft
                        color: palette.text
                    }
                }
            }
        }
    }
}
