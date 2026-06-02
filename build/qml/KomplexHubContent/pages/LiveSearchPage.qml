import QtQuick
import QtQuick.Controls
import KomplexHub
import KomplexHub.Controls
import "../Kero"

Item {
    id: imageSearchPageRoot

    SearchResultList {
        id: searchResultList
        anchors.fill: parent
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
            when: searchResultList.state === "loading"

            PropertyChanges {
                target: loadingOverlay
                opacity: 1
            }
            PropertyChanges {
                target: resultsView
                opacity: 0
            }
        },
        State {
            name: "empty"
            when: searchResultList.state === "empty"

            PropertyChanges {
                target: noResultsOverlay
                opacity: 1
            }
            PropertyChanges {
                target: resultsView
                opacity: 0
            }
        },
        State {
            name: "error"
            when: searchResultList.state === "error"

            PropertyChanges {
                target: errorOverlay
                opacity: 1
            }
            PropertyChanges {
                target: resultsView
                opacity: 0
            }
        }
    ]
}
