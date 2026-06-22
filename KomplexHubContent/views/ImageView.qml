import QtQuick 2.15
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
    property string description
    property string thumbnail
    property string portrait
    property string landscape
    property string small
    property string original
    property string medium
    property string large
    property string extraLarge
    property string portraitSize
    property string landscapeSize
    property string smallSize
    property string originalSize
    property string mediumSize
    property string largeSize
    property string extraLargeSize

    property int imageHeight
    property int imageWidth

    id: rootItem

    Rectangle
    {
        anchors.fill: parent
        color: palette.base

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin
            spacing: Constants.largeMargin

            RowLayout
            {
                Layout.fillWidth: true

                /** Spacer **/
                Item { Layout.fillWidth: true }

                Image
                {
                    width: imageWidth
                    height: imageHeight
                    transform: Image.PreserveAspectFit
                    source: "qrc:/images/icons/pexels-icon-filled-256.svg"
                }

                /** Spacer **/
                Item { Layout.fillWidth: true }
            }

            Text
            {
                id: titleText
                text: rootItem.author

                font.bold: true
                font.pixelSize: Constants.h3Font.pixelSize

                Layout.fillWidth: true
            }

            Text
            {
                id: descriptionText
                text: rootItem.description

                font.pointSize: Constants.h4Font.pixelSize
            }

            Image
            {
                width: 64
                height: 64
                transform: Image.PreserveAspectFit
                source: "qrc:/images/icons/pexels-icon-filled-256.svg"

                Layout.alignment: Qt.AlignBottom | Qt.AlignRight
            }
        }
    }
}
