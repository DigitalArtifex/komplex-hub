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
        color: palette.base.alpha(1)

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin
            spacing: Constants.largeMargin

            RowLayout
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop| Qt.AlignHCenter

                /** Spacer **/
                Item { Layout.fillWidth: true }

                Image
                {
                    width: imageWidth
                    height: imageHeight
                    transform: Image.PreserveAspectFit
                    source: rootItem.large
                }

                /** Spacer **/
                Item { Layout.fillWidth: true }
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
                    model:[
                        "Small: " + smallSize,
                        "Medium: " + mediumSize,
                        "Large: " + largeSize,
                        "Extra Large: " + extraLargeSize,
                        "Portrait: " + portraitSize,
                        "Landscape: " + landscapeSize
                    ]
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

            // Text {
            //     property var texts: [
            //         small,
            //         medium,
            //         large,
            //         extraLarge,
            //         portrait,
            //         landscape
            //     ]
            //     text: texts[downloadSelector.currentIndex]
            //     elide: Text.ElideLeft

            //     Layout.fillWidth: true
            // }

            /** Spacer **/
            Item { Layout.fillWidth: true; Layout.fillHeight: true }

            RowLayout
            {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom

                Text {
                    text: qsTr("Images Provided Courtesy of Pexels")
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
