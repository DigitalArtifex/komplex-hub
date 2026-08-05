import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

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
    onVisibleChanged: () =>
    {
        if(!visible)
        {
            model = null
        }
    }

    Connections
    {
        target: model

        function onRowCountChanged()
        {
            downloadSelector.currentIndex = 0
        }
    }

    id: rootItem

    Rectangle
    {
        anchors.fill: parent
        color: palette.base

        Image
        {
            id: backgroundImage
            source: thumbnail
            anchors.fill: parent
            visible: false
            fillMode: Image.PreserveAspectFit
        }

        MultiEffect
        {
            anchors.fill: backgroundImage
            source: backgroundImage
            blurEnabled: true
            blurMax: 64
            blur: 1.0
            opacity: 0.25
            saturation: -0.8
        }

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin
            spacing: Constants.largeMargin

            RowLayout
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop| Qt.AlignHCenter
                Layout.topMargin: 32

                Rectangle
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width / 1.77777777778
                    Layout.maximumWidth: 800
                    Layout.alignment: Qt.AlignHCenter
                    color: palette.base.darker()

                    Throbber
                    {
                        anchors.fill: parent
                        id: loadingThrobber
                        visible: viewImage.status === Image.Loading
                    }

                    Image
                    {
                        id: viewImage
                        anchors.fill: parent
                        source: rootItem.thumbnail
                        visible: !(status === Image.Loading)
                        fillMode: Image.PreserveAspectCrop
                    }
                }
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
    }
}
