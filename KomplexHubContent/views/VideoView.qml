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
import QtQuick.Effects

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin

Item
{
    property string uuid
    property string author
    property string authorUrl
    property string thumbnail
    property alias model: downloadSelector.model
    property int modelIndex

    property int imageHeight
    property int imageWidth

    visible: opacity > 0.01

    Connections
    {
        target: model ? model : null

        function onRowCountChanged()
        {
            downloadSelector.currentIndex = 0
        }
    }

    DownloadManager
    {
        id: downloadManager
    }

    id: rootItem

    Rectangle
    {
        id: pageView
        anchors.fill: parent
        color: palette.base

        BackgroundImage
        {
            anchors.fill: parent
            source: rootItem.thumbnail
        }

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin
            spacing: Constants.largeMargin

            ImageFrame
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop| Qt.AlignHCenter
                Layout.topMargin: Constants.largeMargin
                Layout.preferredHeight: width / 1.77777777778
                Layout.maximumWidth: 800

                source: rootItem.thumbnail
            }

            Text
            {
                id: titleText
                text: rootItem.author
                color: palette.text
                font.bold: true
                font.pixelSize: Constants.h3Font.pixelSize
                elide: Text.ElideRight

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop| Qt.AlignLeft
            }

            /** Spacer **/
            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 32
            }

            Text
            {
                text: qsTr("Available Downloads")
                color: palette.text
                font.bold: true
                font.pixelSize: Constants.h3Font.pixelSize
                elide: Text.ElideRight

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop| Qt.AlignLeft
            }

            RowLayout
            {
                Layout.fillWidth: true

                ComboBox
                {
                    id: downloadSelector
                    textRole: "text"
                    valueRole: "url"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                }
                SquareButton
                {
                    Layout.preferredHeight: 36
                    Layout.preferredWidth: 128
                    text: qsTr("Download")
                    icon.source: "qrc:/images/icons/icons8-download.svg"

                    onTriggered: () => rootItem.download()
                }
            }

            /** Spacer **/
            Item { Layout.fillWidth: true; Layout.fillHeight: true }

            RowLayout
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom

                Text {
                    text: qsTr("Videos Provided Courtesy of Pexels and Their Respective Authors")
                    elide: Text.ElideLeft
                    color: palette.text
                    font.bold: true
                    font.pixelSize: Constants.h6Font.pixelSize

                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                }

                Image
                {
                    transform: Image.PreserveAspectFit
                    source: "qrc:/images/icons/pexels-icon-filled-256.svg"

                    Layout.alignment: Qt.AlignRight
                    Layout.preferredHeight: 32
                    Layout.preferredWidth: 32
                }
            }
        }

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 250
            }
        }

        states:
        [
            State
            {
                name: "downloading"
                when: downloadManager.state === DownloadManager.Downloading

                PropertyChanges
                {
                    target: pageView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: downloadView
                    opacity: 1
                }

                PropertyChanges
                {
                    target: completedView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: errorView
                    opacity: 0
                }
            },
            State
            {
                name: "complete"
                when: downloadManager.state === DownloadManager.Complete

                PropertyChanges
                {
                    target: pageView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: downloadView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: completedView
                    opacity: 1
                }

                PropertyChanges
                {
                    target: errorView
                    opacity: 0
                }
            },
            State
            {
                name: "error"
                when: downloadManager.state === DownloadManager.Error

                PropertyChanges
                {
                    target: pageView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: downloadView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: completedView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: errorView
                    opacity: 1
                }
            },
            State
            {
                name: "idle"
                when: downloadManager.state === DownloadManager.Idle

                PropertyChanges
                {
                    target: pageView
                    opacity: 1
                }

                PropertyChanges
                {
                    target: downloadView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: completedView
                    opacity: 0
                }

                PropertyChanges
                {
                    target: errorView
                    opacity: 0
                }
            }
        ]
    }

    DownloadView
    {
        id: downloadView
        anchors.fill: parent

        manager: downloadManager
        thumbnail: rootItem.thumbnail
        opacity: 0
        visible: opacity > 0.01

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 250
            }
        }
    }

    CompletedView
    {
        id: completedView
        anchors.fill: parent

        opacity: 0
        visible: opacity > 0.01
        manager: downloadManager

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 250
            }
        }
    }

    ErrorView
    {
        id: errorView
        anchors.fill: parent

        opacity: 0
        visible: opacity > 0.01
        manager: downloadManager

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 250
            }
        }
    }

    function download()
    {
        downloadManager.downloadImage(author, authorUrl, downloadSelector.currentValue)
    }
}
