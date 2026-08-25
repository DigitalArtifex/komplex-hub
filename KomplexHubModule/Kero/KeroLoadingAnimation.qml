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
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    property string title: qsTr("Loading")
    property string description: qsTr("Kero is busy looking for wallpapers..we promise")
    property color color: palette.window

    id: keroLoadingOverlayRoot

    Rectangle {
        anchors.fill: parent
        color: keroLoadingOverlayRoot.color

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 64
                Layout.minimumHeight: 64
                Layout.maximumWidth: 512
                Layout.maximumHeight: 512
                Layout.alignment: Qt.AlignCenter

                color: "transparent"

                Image {
                    id: keroLoadingImage
                    anchors.fill: parent

                    source: "qrc:/images/kero/kero_run_1.png"
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit

                    Timer {
                        property int frame: 1
                        id: animationTimer
                        interval: 175
                        running: parent.visible
                        repeat: true

                        onTriggered: () => {
                            frame += 1;

                            if(frame >= 4)
                                frame = 1

                            keroLoadingImage.source = "qrc:/images/kero/kero_run_" + frame + ".png"
                        }
                    }
                }

                ColumnLayout {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Text {
                        color: palette.text
                        text: keroLoadingOverlayRoot.title
                        font.pixelSize: Constants.largeFont.pixelSize * 1.5

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    }

                    Text {
                        id: errorText
                        text: keroLoadingOverlayRoot.description
                        color: palette.text.darker()

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.bottomMargin: Constants.largeMargin
                    }
                }
            }
        }
    }
}
