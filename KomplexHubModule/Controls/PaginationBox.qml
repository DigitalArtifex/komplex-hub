import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import KomplexHub

Item
{
    id: itemRoot

    property int page: 0
    property int totalPages: 0

    property bool enablePrevious: false
    property bool enableNext: false

    signal next
    signal previous

    RowLayout
    {
        id: buttonBoxLayout
        anchors.fill: parent

        SquareButton
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 128

            icon.source: "qrc:/images/icons/icons8-back.svg"
            icon.height: 16
            icon.width: 16
            text: qsTr("Previous")
            enabled: itemRoot.enablePrevious

            onTriggered: () => itemRoot.previous()
        }

        Item { Layout.fillWidth: true }

        SquareButton
        {
            Layout.fillHeight: true
            Layout.preferredWidth: 128

            icon.source: "qrc:/images/icons/icons8-next.svg"
            icon.height: 16
            icon.width: 16
            text: qsTr("Next")
            enabled: itemRoot.enableNext

            onTriggered: () => itemRoot.next()
        }
    }
}
