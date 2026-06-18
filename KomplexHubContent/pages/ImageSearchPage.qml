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
    property int resultsPerRow: (resultsView.width - (64 + Constants.largeMargin)) / (256 + Constants.largeMargin)
    property int rows: (resultsView.height - (Constants.largeMargin * 2) / (256 + Constants.largeMargin))
    property int resultsPerPage: resultsPerRow * rows

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

        ScrollView
        {
            id: resultsView
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: buttonBoxLayout.top

            GridLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.top: parent.top

                anchors.margins: Constants.largeMargin
                columns: imageSearchPageRoot.resultsPerRow

                Repeater {
                    model: searchModel

                    delegate: Item {
                        width: 256
                        height: 256
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

        RowLayout {
            id: buttonBoxLayout
            height: 50

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            SquareButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 128

                icon.source: "qrc:/images/icons/icons8-back.svg"
                icon.height: 16
                icon.width: 16
                text: qsTr("Previous")
                enabled: searchModel.hasPreviousPage
            }

            Item { Layout.fillWidth: true }

            SquareButton {
                Layout.fillHeight: true
                Layout.preferredWidth: 128

                icon.source: "qrc:/images/icons/icons8-next.svg"
                icon.height: 16
                icon.width: 16
                text: qsTr("Next")
                enabled: searchModel.hasNextPage
            }
        }
    }

    KeroErrorAvatar {
        id: errorOverlay

        anchors.fill: parent
        opacity: 0
    }

    KeroNoResultsAvatar {
        id: noResultsOverlay

        anchors.fill: parent
        opacity: 0
    }

    KeroLoadingAnimation {
        id: loadingOverlay

        anchors.fill: parent
        opacity: 0
    }

    states: [
        State {
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
        State {
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
            PropertyChanges {
                target: buttonBoxLayout
                visible: false
            }
        },
        State {
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
