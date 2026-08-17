import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Plugin
import KomplexHub.Kero

Item
{
    property string thumbnail
    property DownloadManager manager

    Rectangle
    {
        anchors.fill: parent
        color: palette.base

        BackgroundImage
        {
            source: thumbnail
        }

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin

            Item { Layout.preferredHeight: 64; Layout.fillWidth: true }

            KeroBuildingAnimation
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            VerticalSpacer {}

            Text
            {
                text: qsTr("Download Progress")
                font: Constants.h3Font
                color : palette.text
                Layout.preferredHeight: Constants.largeMargin
            }

            RowLayout
            {
                Layout.fillWidth: true
                Layout.preferredHeight: Constants.largeMargin

                ProgressBar
                {
                    Layout.fillWidth: true
                    value: manager.downloadProgress
                }

                Text
                {
                    text: manager.downloadedBytes
                    font: Constants.h4Font
                    color : palette.text
                }

                Text
                {
                    text: "/"
                    font: Constants.h4Font
                    color : palette.text
                    Layout.preferredHeight: 32
                }

                Text
                {
                    text: manager.downloadSize
                    font: Constants.h4Font
                    color : palette.text
                    Layout.preferredHeight: 32
                }
            }

            Text
            {
                text: qsTr("Install Progress")
                font: Constants.h3Font
                color : palette.text
                height: 32
            }

            RowLayout
            {
                Layout.fillWidth: true
                Layout.preferredHeight: Constants.largeMargin

                ProgressBar
                {
                    Layout.fillWidth: true
                    value: manager.compileProgress
                }

                Text
                {
                    text: manager.compileStepsCompleted
                    font: Constants.h4Font
                    color : palette.text
                    Layout.preferredHeight: Constants.largeMargin
                }

                Text
                {
                    text: "/"
                    font: Constants.h4Font
                    color : palette.text
                    Layout.preferredHeight: Constants.largeMargin
                }

                Text
                {
                    text: manager.compileSteps
                    font: Constants.h4Font
                    color : palette.text
                    Layout.preferredHeight: Constants.largeMargin
                }
            }

            Item { Layout.preferredHeight: Constants.largeMargin * 2; Layout.fillWidth: true }
        }
    }
}
