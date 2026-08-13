import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Item
{
    readonly property bool searchable: false
    property int resultsPerRow: (resultsContainer.width - (Constants.largeMargin * 2)) / (resultWidth + Constants.mediumMargin)
    property int rows: ((resultsContainer.height - (Constants.largeMargin * 2)) / (resultHeight + Constants.mediumMargin))
    property int resultsPerPage: resultsPerRow * rows
    property int emptyWidth: (resultsContainer.width - (Constants.largeMargin * 2)) - (resultsPerRow * (resultWidth + Constants.mediumMargin))
    property int resultWidth: 256
    property int resultHeight: 245
    property bool loading: false

    property alias currentItem: resultsView.currentItem
    property alias currentIndex: resultsView.currentIndex

    property var model

    property Component delegate

    signal viewMoreTriggered()

    id: rootItem

    Item
    {
        id: resultsContainer
        anchors.fill: parent

        RowLayout
        {
            anchors.fill: parent

            Component
            {
                id: highlight
                Rectangle
                {
                    width: 256
                    height: 245
                    color: palette.accent
                    radius: 5
                    // x: resultsView.currentItem.x
                    // y: resultsView.currentItem.y
                    Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                    Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
                }
            }

            ListView
            {
                id: resultsView
                Layout.fillHeight: true
                Layout.fillWidth: true

                spacing: Constants.mediumMargin

                orientation: ListView.Horizontal
                model: rootItem.model

                delegate: rootItem.delegate
                highlight: Rectangle { color: palette.accent; radius: 5 }
                highlightFollowsCurrentItem: true
                focus: true

                add: Transition
                {
                    NumberAnimation
                    {
                        properties: "x"
                        from: resultsContainer.width
                        duration: 250
                    }
                    NumberAnimation
                    {
                        properties: "opacity,scale"
                        to: 1.0
                        duration: 500
                    }
                }

                move: Transition
                {
                    NumberAnimation
                    {
                        properties: "x,y"
                        duration: 250
                    }
                }

                displaced: Transition
                {
                    NumberAnimation
                    {
                        properties: "x,y"
                        duration: 250
                    }
                }

                populate: Transition
                {
                    NumberAnimation
                    {
                        properties: "opacity,scale"
                        to: 1.0
                        duration: 500
                    }
                }

                remove: Transition
                {
                    NumberAnimation
                    {
                        properties: "x"
                        to: 0
                        duration: 250
                    }
                    NumberAnimation
                    {
                        properties: "opacity"
                        to: 0
                        duration: 250
                    }
                }
            }

            Item
            {
                Layout.fillHeight: true
                Layout.preferredWidth: 128
                Rectangle
                {
                    id: gradientMap
                    anchors.top: parent.top
                    anchors.right: moreButton.left
                    height: resultHeight
                    width: resultWidth / 4

                    gradient: Gradient
                    {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: "transparent" }
                        GradientStop { position: 1.0; color: palette.light }
                    }
                    opacity: 0.5
                }

                SquareButton
                {
                    id: moreButton
                    anchors.top: parent.top
                    anchors.right: parent.right
                    height: resultHeight

                    width: 64

                    icon.source: "qrc:/images/icons/icons8-forward.svg"
                    icon.height: 24
                    icon.width: 24

                    onTriggered: () => viewMoreTriggered()
                }
            }
        }
    }
}
