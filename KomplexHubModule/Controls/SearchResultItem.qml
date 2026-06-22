import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub

Item {
    property string author: ""
    property string description: ""
    property string title: ""
    property string thumbnail: ""
    property string uuid: ""
    property bool selected: false
    property bool pexels: false

    signal triggered

    id: searchResultItemRoot
    width: 256
    height: 256
    clip: true

    Rectangle
    {
        id: searchResultItemContainer
        anchors.fill: parent

        border.color: palette.alternateBase.lighter(1.75)

        color: palette.alternateBase.lighter(1.25)

        ColumnLayout
        {
            anchors.fill: parent

            Rectangle
            {
                Layout.preferredWidth: 250
                Layout.preferredHeight: 141
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                Layout.topMargin: 3

                border.color: palette.alternateBase.lighter(1.75)

                color: palette.base.lighter(1.25)

                Image
                {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    id: thumnailImage
                    source: searchResultItemRoot.thumbnail
                }

                Image
                {
                    antialiasing: true
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.margins: Constants.mediumMargin

                    width: 24
                    height: 24
                    transform: Image.PreserveAspectFit
                    opacity: 0.5
                    source: "qrc:/images/icons/pexels-icon-filled-256.svg"

                    visible: searchResultItemRoot.pexels
                }
            }

            Text {
                color: palette.text
                text: searchResultItemRoot.title

                font.pixelSize: 16
                font.bold: true
                elide: Text.ElideRight

                Layout.preferredWidth: 246
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            }

            Item { Layout.fillHeight: true }

            Text {
                color: palette.text
                font.pixelSize: 12
                text: "By: " + searchResultItemRoot.author
                leftPadding: 12

                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                Layout.preferredWidth: 250
                visible: !searchResultItemRoot.pexels
            }

            Text {
                clip: true
                color: palette.text
                font.pixelSize: 12
                text: searchResultItemRoot.description
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                textFormat: Text.PlainText
                Layout.maximumHeight: 32
                Layout.preferredHeight: 32
                elide: Text.ElideRight

                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Layout.preferredWidth: 236
                Layout.maximumWidth: 236
                Layout.bottomMargin: 3
            }

            Item { Layout.fillHeight: true }
        }

        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }

    MouseArea {
        property string lastState
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true

        onEntered: () => {
            if(!selected)
                searchResultItemRoot.state = "hovered"
        }

        onExited: () => {
            lastState = ""

            if(selected)
                searchResultItemRoot.state = "selected"
            else
                searchResultItemRoot.state = ""
        }

        onPressed: () => {
            if(!selected) {
                lastState = searchResultItemRoot.state
                searchResultItemRoot.state = "clicked"
            }
        }

        onReleased: () => {
            if(!mouseArea.containsMouse)
                return;

            selected = !selected
        }
    }

    onSelectedChanged: () => {
        if(selected) {
            searchResultItemRoot.triggered()
            searchResultItemRoot.state = "selected"
        }
        else {
            searchResultItemRoot.state = ""
        }
    }

    states: [
        State {
            name: "hovered"

            PropertyChanges {
                target: searchResultItemContainer
                color: palette.alternateBase.lighter(1.5)
                border.color: Qt.hsva(palette.accent.hslHue,
                                      palette.accent.hslSaturation,
                                      0.5, // Set custom lightness
                                      1.0)
            }
        },
        State {
            name: "selected"

            PropertyChanges {
                target: searchResultItemContainer
                color: palette.accent
            }
        }
    ]
}
