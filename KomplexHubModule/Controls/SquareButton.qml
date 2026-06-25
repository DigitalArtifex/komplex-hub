import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    property string text: qsTr("")
    property IconSettings icon: IconSettings {}
    property alias spacing: squareButtonLayout.spacing
    property int margins: 4
    property bool checkable: false
    property bool checked: false
    property color color: palette.button
    property Gradient gradient: defaultGradient
    property BorderSettings border: BorderSettings {}
    readonly property bool hovered: state === "hovered"

    signal triggered

    height: 36
    width: 180

    id: squareButtonRoot

    //icon properties. access them after Component.onLoad()
    component IconSettings: QtObject {
        property int width: 16
        property int height: 16
        property string source: "images/icons/icons8-next.svg"
    }

    component BorderSettings: QtObject {
    }

    Gradient {
        id: clickedGradient
        GradientStop { position: 0.0; color: palette.button.darker(1.25) }
        GradientStop { position: 1.0; color: palette.button }
    }

    Gradient {
        id: defaultGradient
        GradientStop { position: 0.0; color: palette.button.lighter(1.25) }
        GradientStop { position: 1.0; color: palette.button }
    }

    Gradient {
        id: hoveredGradient
        GradientStop { position: 0.0; color: palette.button.lighter(1.5) }
        GradientStop { position: 1.0; color: palette.button.lighter(1.25) }
    }

    Rectangle {
        id: squareButtonBackground
        anchors.fill: parent
        gradient: squareButtonRoot.gradient
        border.color: palette.button.lighter()

        RowLayout {
            id: squareButtonLayout

            anchors.fill: parent

            Rectangle {
                Layout.preferredHeight: squareButtonRoot.height < squareButtonRoot.width ? squareButtonRoot.height : squareButtonRoot.width
                Layout.preferredWidth: squareButtonRoot.height < squareButtonRoot.width ? squareButtonRoot.height : squareButtonRoot.width

                id: iconContainer

                color: "transparent"

                ColumnLayout {
                    anchors.fill: parent

                    Image {
                        id: squareButtonIcon

                        Layout.preferredWidth: icon.width
                        Layout.preferredHeight: icon.height
                        Layout.maximumHeight: icon.height
                        Layout.maximumWidth: icon.width

                        source: icon.source
                        fillMode: Image.PreserveAspectFit
                        Layout.alignment: Qt.AlignCenter
                    }
                }
            }

            Text {
                id: squareButtonText
                color: palette.text
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
                text: squareButtonRoot.text

                font.pixelSize: 14

                visible: text.length > 0

                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: iconContainer.width * -1
            }
        }

        Behavior on color {
            ColorAnimation { duration: 250 }
        }
    }

    MouseArea {
        property string lastState
        id: mouseArea

        anchors.fill: parent
        hoverEnabled: true

        onEntered: () => {
            squareButtonRoot.state = "hovered"
        }

        onExited: () => {
            lastState = ""

            if(checked)
                squareButtonRoot.state = "checked"
            else
                squareButtonRoot.state = ""
        }

        onPressed: () => {
            lastState = squareButtonRoot.state
            squareButtonRoot.state = "clicked"
        }

        onReleased: () => {
            if(!mouseArea.containsMouse)
                return;

            squareButtonRoot.state = lastState

            if(checkable)
                checked = !checked

            triggered()
        }
    }

    onCheckedChanged: () => {
        if(checked) {
            squareButtonRoot.triggered()
            squareButtonRoot.state = "checked"
        }
        else {
            squareButtonRoot.state = ""
        }
    }

    states:[
        State {
            name: "hovered"
            PropertyChanges {
                target: squareButtonBackground
                border.color: palette.accent
                gradient: hoveredGradient
            }

            PropertyChanges {
                target: squareButtonText
                color: palette.text
            }
        },

        State {
            name: "clicked"
            PropertyChanges {
                target: squareButtonBackground
                border.color: palette.accent.darker(1.5)
                gradient: clickedGradient
            }

            PropertyChanges {
                target: squareButtonText
                color: palette.text
            }
        },

        State {
            name: "checked"
            PropertyChanges {
                target: squareButtonBackground
                border.color: palette.accent.darker(1.5)
                gradient: clickedGradient
            }

            PropertyChanges {
                target: squareButtonText
                color: palette.text
            }
        },

        State {
            name: "disabled"
            PropertyChanges {
                target: squareButtonBackground
                color: palette.button.darker()
                gradient: null
            }

            PropertyChanges {
                target: squareButtonText
                color: palette.text.darker()
            }
        }
    ]
}
