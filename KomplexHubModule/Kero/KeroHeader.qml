import QtQuick
import QtQuick.Controls 2.0
import QtQuick.Layouts
import KomplexHub

Item
{
    property string title: qsTr("Wallpaper Spotlight")
    property string description: qsTr("Featured Images and Live Wallpaper Packs")
    property color color: palette.alternateBase
    property string image: "qrc:/images/kero/kero_spotlight.png"

    id: rootItem

    Rectangle
    {
        anchors.fill: parent
        color: rootItem.color
        border.color: palette.midlight

        RowLayout
        {
            anchors.fill: parent

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Rectangle
            {
                Layout.fillHeight: true
                Layout.preferredWidth: height
                Layout.alignment: Qt.AlignLeft

                color: "transparent"

                Image
                {
                    anchors.fill: parent

                    source: image
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit
                }
            }

            ColumnLayout
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft

                Text
                {
                    color: palette.text
                    text: rootItem.title
                    font: Constants.h1Font

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                }

                Text
                {
                    id: errorText
                    text: rootItem.description
                    color: palette.text.darker()

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    Layout.bottomMargin: Constants.largeMargin
                }
            }

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
