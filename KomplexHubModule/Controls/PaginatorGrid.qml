import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Item
{
    property string query
    readonly property bool searchable: true
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

    id: rootItem

    KeroErrorAvatar
    {
        id: errorOverlay

        anchors.fill: parent
        opacity: 0
    }

    KeroNoResultsAvatar
    {
        id: noResultsOverlay

        anchors.fill: parent
        opacity: 0
    }

    KeroChoiceAvatar
    {
        id: noSearchOverlay

        title: qsTr("No Search Term")
        description: qsTr("Kero needs a subject or term to search for wallpapers")

        anchors.fill: parent
        opacity: 0
    }

    KeroLoadingAnimation
    {
        id: loadingOverlay

        anchors.fill: parent
        opacity: 0
    }

    Rectangle
    {
        id: resultsContainer
        anchors.fill: parent
        color: palette.base

        RowLayout
        {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin

            Item
            {
                Layout.fillHeight: true
                Layout.preferredWidth: emptyWidth / 2
                Layout.maximumWidth: emptyWidth / 2
            }

            Component
            {
                id: highlight
                Rectangle
                {
                    width: view.cellWidth
                    height: view.cellHeight
                    color: palette.accent
                    radius: 5
                    x: view.currentItem.x
                    y: view.currentItem.y
                    Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                    Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
                }
            }

            GridView
            {
                id: resultsView
                Layout.fillHeight: true
                Layout.fillWidth: true

                cellWidth: 256 + Constants.mediumMargin
                cellHeight: 245 + Constants.mediumMargin
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
        }

        PaginationBox
        {
            id: buttonBoxLayout
            height: 35

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: Constants.largeMargin

            onNext: () => model.nextPage()
            onPrevious: () => model.previousPage()

            enablePrevious: model.hasPreviousPage
            enableNext: model.hasNextPage
        }
    }

    states: [
        State
        {
            name: "loading"
            when: loading

            PropertyChanges
            {
                target: loadingOverlay
                opacity: 1
            }
            PropertyChanges
            {
                target: resultsContainer
                opacity: 0
            }
            PropertyChanges
            {
                target: noSearchOverlay
                opacity: 0
            }
            PropertyChanges
            {
                target: errorOverlay
                opacity: 0
            }
            PropertyChanges
            {
                target: buttonBoxLayout
                visible: false
            }
        },
        State
        {
            name: "empty"
            when: model.totalResults === 0 && model.query.length > 0

            PropertyChanges
            {
                target: noResultsOverlay
                opacity: 1
            }
            PropertyChanges
            {
                target: noSearchOverlay
                opacity: 0
            }
            PropertyChanges
            {
                target: resultsContainer
                opacity: 0
            }
            PropertyChanges
            {
                target: buttonBoxLayout
                visible: false
            }
        },
        State
        {
            name: "nosearch"
            when: model.query.length === 0

            PropertyChanges
            {
                target: noResultsOverlay
                opacity: 0
            }
            PropertyChanges
            {
                target: noSearchOverlay
                opacity: 1
            }
            PropertyChanges
            {
                target: resultsContainer
                opacity: 0
            }
            PropertyChanges
            {
                target: buttonBoxLayout
                visible: false
            }
        },
        State
        {
            name: "idle"
            when: model.totalResults !== 0 && !loading

            PropertyChanges
            {
                target: loadingOverlay
                opacity: 0.0
            }
            PropertyChanges
            {
                target: resultsContainer
                opacity: 1.0
            }
            PropertyChanges
            {
                target: buttonBoxLayout
                visible: true
            }
        },
        State
        {
            name: "error"
            when: model.errorString.length > 0

            PropertyChanges
            {
                target: errorOverlay
                opacity: 1.0
            }
            PropertyChanges
            {
                target: resultsContainer
                opacity: 0.0
            }
            PropertyChanges
            {
                target: noSearchOverlay
                opacity: 0.0
            }
            PropertyChanges
            {
                target: noResultsOverlay
                opacity: 0.0
            }
        }
    ]

    transitions: [
        Transition
        {
            from: "empty"
            to: "loading"
            NumberAnimation
            {
                target: noResultsOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
            NumberAnimation
            {
                target: loadingOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
        },
        Transition
        {
            from: "nosearch"
            to: "loading"
            NumberAnimation
            {
                target: noSearchOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
            NumberAnimation
            {
                target: loadingOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
        },
        Transition
        {
            from: "loading"
            to: "nosearch"
            NumberAnimation
            {
                target: noSearchOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
            NumberAnimation
            {
                target: loadingOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
        },
        Transition
        {
            from: "loading"
            to: "empty"
            NumberAnimation
            {
                target: noResultsOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
            NumberAnimation
            {
                target: loadingOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
        },
        Transition
        {
            from: "loading"
            to: "idle"
            NumberAnimation
            {
                target: loadingOverlay
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
            NumberAnimation
            {
                target: resultsView
                duration: 250
                property: "opacity";
                easing.type: Easing.InOutQuad
            }
        }
    ]
}
