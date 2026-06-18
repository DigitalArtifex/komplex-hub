/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    property string searchTerm: ""
    property string icon: ""
    property color color: "transparent"

    id: searchBarRootItem
    clip: true

    Rectangle {
        color: searchBarRootItem.color
        anchors.fill: parent

        RowLayout {
            id: searchContainerLayout
            anchors.fill: parent

            LineEdit {
                id: searchEdit
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: Constants.mediumMargin
                Layout.rightMargin: Constants.mediumMargin
                Layout.topMargin: Constants.smallMargin
                Layout.bottomMargin: Constants.smallMargin
                padding.top: 12

                font.pixelSize: 12

                onAccepted: () => searchBarRootItem.searchTerm = text
            }

            SquareButton {
                id: searchButton

                Layout.fillHeight: searchEdit.height
                Layout.preferredWidth: searchEdit.height
                Layout.rightMargin: Constants.mediumMargin
                Layout.topMargin: Constants.mediumMargin
                Layout.bottomMargin: Constants.mediumMargin

                icon.source: searchBarRootItem.icon
                icon.width: 16
                icon.height: 16

                onTriggered: () => searchBarRootItem.searchTerm = searchEdit.text
            }
        }
    }

    states: [
        State {
            name: "hidden"
            PropertyChanges {
                target: searchBarRootItem
                visible: false
            }
        }
    ]
}
