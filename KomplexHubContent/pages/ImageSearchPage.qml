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

    id: imageSearchPageRoot

    ImageSearchModel
    {
        id: searchModel
        query: imageSearchPageRoot.query
        resultsPerPage: imageSearchPageRoot.resultsPerPage
    }

    Rectangle
    {
        anchors.fill: parent
        color: palette.base

        Rectangle
        {
            id: resultsContainer
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: buttonBoxLayout.top

            color: "transparent"

            ScrollView
            {
                id: resultsView
                anchors.fill: parent

                RowLayout
                {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.margins: Constants.largeMargin

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.preferredWidth: emptyWidth / 2
                        Layout.maximumWidth: emptyWidth / 2
                    }

                    GridLayout
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        columns: imageSearchPageRoot.resultsPerRow
                        columnSpacing: Constants.mediumMargin
                        rowSpacing: Constants.mediumMargin

                        Repeater
                        {
                            model: searchModel

                            delegate: Item
                            {
                                width: 256
                                height: 245
                                required property string author
                                required property string description
                                required property string uuid
                                required property string thumbnail
                                required property string authorId

                                SearchResultItem
                                {
                                    anchors.fill: parent
                                    title: parent.author
                                    author: qsTr("Pexels Images")
                                    description: parent.description
                                    thumbnail: parent.thumbnail
                                    uuid: parent.uuid
                                }
                            }
                        }
                    }
                }
            }
        }

        RowLayout
        {
            id: buttonBoxLayout
            height: 35

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: Constants.largeMargin

            SquareButton
            {
                Layout.fillHeight: true
                Layout.preferredWidth: 128

                icon.source: "qrc:/images/icons/icons8-back.svg"
                icon.height: 16
                icon.width: 16
                text: qsTr("Previous")
                enabled: searchModel.hasPreviousPage

                onTriggered: () => searchModel.previousPage()
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
                enabled: searchModel.hasNextPage

                onTriggered: () => searchModel.nextPage()
            }
        }
    }

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

    KeroLoadingAnimation
    {
        id: loadingOverlay

        anchors.fill: parent
        opacity: 0
    }

    states: [
        State
        {
            name: "loading"
            when: searchModel.state == ImageSearchModel.Loading

            PropertyChanges
            {
                target: loadingOverlay
                opacity: 1
            }
            PropertyChanges
            {
                target: resultsView
                opacity: 0
            }
        },
        State
        {
            name: "empty"
            when: searchModel.totalResults === 0

            PropertyChanges
            {
                target: noResultsOverlay
                opacity: 1
            }
            PropertyChanges
            {
                target: resultsView
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
            name: "error"
            when: searchModel.state == ImageSearchModel.Error

            PropertyChanges
            {
                target: errorOverlay
                opacity: 1
            }
            PropertyChanges
            {
                target: resultsView
                opacity: 0
            }
        }
    ]
}
