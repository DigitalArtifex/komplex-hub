import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Kero
import KomplexHub.Controls
import KomplexHub.Plugin

Item
{
    property DownloadManager manager: null

    ColumnLayout
    {
        anchors.fill: parent

        VerticalSpacer { }

        KeroWarningAvatar
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 512
            Layout.maximumHeight: 512
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            Layout.maximumHeight: 36
            Layout.margins: Constants.largeMargin

            Item
            {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            SquareButton
            {
                Layout.preferredWidth: 186
                Layout.fillHeight: true
                text: qsTr("Okay")

                onTriggered: () =>
                {
                    if(manager != null)
                    {
                        manager.reset()
                    }
                }
            }
        }

        VerticalSpacer {}
    }
}
