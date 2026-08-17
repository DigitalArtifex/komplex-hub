import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item
{
    property alias source: viewImage.source
    id: rootItem

    Rectangle
    {
        anchors.fill: parent
        Layout.alignment: Qt.AlignCenter
        color: palette.base.darker()
        border.color: palette.midlight

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
            transform: Image.PreserveAspectCrop
            source: rootItem.source
            visible: !(status === Image.Loading)
        }
    }

}
