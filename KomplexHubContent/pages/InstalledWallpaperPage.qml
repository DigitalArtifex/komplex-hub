import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin
import KomplexHub.Views
import KomplexHub.Pages

Rectangle {
    id: root

    color: palette.base

    ListView {
        id: view
        anchors.fill: parent

        InstalledPackManager {
            id: manager
        }

        model: manager
        delegate: Rectangle {
            required property string name
            required property string author
            required property string description
            required property string thumbnail
            required property string uri

            width: view.width
            height: 50

            border.color: palette.base.lighter()

            color: "transparent"

            RowLayout {
                anchors.fill: parent

                Rectangle {
                    color: palette.base.darker()
                    border.color: palette.base.lighter()
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 35
                    Layout.leftMargin: 12

                    Image {
                        anchors.fill: parent
                        source: thumbnail
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true

                    Text {
                        Layout.fillWidth: true
                        text: name
                        color: palette.text
                    }
                    Text {
                        Layout.fillWidth: true
                        text: author
                        color: palette.text
                    }
                }

                SquareButton {
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 35
                    Layout.alignment: Qt.AlignRight
                    Layout.rightMargin: 12
                    icon.source: "qrc:/images/icons/icons8-trash.svg"

                    onTriggered: () => {
                        askToUninstall(uri)
                    }
                }
            }
        }
    }

    function askToUninstall(uri) {
        uninstallMessageDialog.uri = uri
        uninstallMessageDialog.visible = true
    }

    function uninstall(uri) {
        if(!manager.uninstall(uri));
    }

    MessageDialog {
        property string uri: ""

        id: uninstallMessageDialog
        text: qsTr("Wallpaper Pack Will Be Uninstalled")
        informativeText: qsTr("This action cannot be undone. Proceed?")
        buttons: MessageDialog.Ok | MessageDialog.Cancel
        onButtonClicked: (button, role) => {
            switch (button) {
            case MessageDialog.Ok:
                if(!uninstall(uri))
                    uninstallErrorDialog.visible = true
                else
                    manager.rescan()
                break;
            }
        }
    }

    MessageDialog {
        id: uninstallErrorDialog
        text: qsTr("Wallpaper Failed To Uninstall")
        informativeText: manager.errorString
        buttons: MessageDialog.Ok
    }

    KeroLoadingAnimation {
        id: loadingAnimation

        anchors.fill: parent
        visible: false
        color: palette.base.alpha(1)
    }

    states:[
        State {
            name: "loading"
            when: manager.state == InstalledPackManager.Loading
            PropertyChanges {
                target: loadingAnimation
                visible: true
            }
        }

    ]
}