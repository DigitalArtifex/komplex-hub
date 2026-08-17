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

    property int imageHeight
    property int imageWidth

    id: rootItem

    Rectangle
    {
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
                }
            }

            /** Spacer **/
            Item { Layout.fillWidth: true; Layout.fillHeight: true }

            RowLayout
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom

                Text {
                    text: qsTr("Shaders Provided Courtesy of ShaderToy and Their Respective Authors")
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
