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
    property string authorId
    property string description
    property string thumbnail
    property string name

    property int imageHeight
    property int imageWidth

    id: rootItem

    DownloadManager
    {
        id: downloadManager
    }

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
                text: rootItem.name
                color: palette.text
                font.bold: true
                font.pixelSize: Constants.h3Font.pixelSize
                elide: Text.ElideRight

                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop| Qt.AlignLeft
            }

            Text
            {
                id: descriptionText
                text: rootItem.description
                color: palette.text
                font.pointSize: Constants.h4Font.pixelSize
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                Layout.alignment: Qt.AlignTop| Qt.AlignLeft
                Layout.fillWidth: true
            }

            Text
            {
                id: authorText
                text: qsTr("By: ") + rootItem.author
                color: palette.text
                font.pointSize: Constants.h5Font.pixelSize
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Qt.AlignRight

                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                Layout.fillWidth: true
            }

            /** Spacer **/
            Item
            {
                Layout.fillWidth: true
                Layout.preferredHeight: 32
            }

            RowLayout
            {
                Layout.fillWidth: true

                Item { Layout.fillWidth: true }

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
                    text: qsTr("Shaders Provided Courtesy of ShaderToy, Komplex and Their Respective Authors")
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
    }

    CompletedView
    {
        id: completedView
        anchors.fill: parent

        opacity: 0
        visible: opacity > 0.01
        manager: downloadManager
    }

    ErrorView
    {
        id: errorView
        anchors.fill: parent

        opacity: 0
        visible: opacity > 0.01
        manager: downloadManager
    }

    function download()
    {
        downloadManager.downloadPack(uuid)
    }

    Component.onCompleted: () => downloadManager.reset()
}
