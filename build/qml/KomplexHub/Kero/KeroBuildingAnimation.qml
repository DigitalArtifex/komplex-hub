import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    property string title: qsTr("Loading")
    property string description: qsTr("Kero is busy looking for wallpapers..we promise")
    property color color: palette.window

    id: keroLoadingOverlayRoot

    Rectangle {
        anchors.fill: parent
        color: keroLoadingOverlayRoot.color

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.minimumWidth: 64
                Layout.minimumHeight: 64
                Layout.maximumWidth: 512
                Layout.maximumHeight: 512
                Layout.alignment: Qt.AlignCenter

                color: "transparent"

                Image {
                    id: keroLoadingImage
                    anchors.fill: parent

                    source: "../images/kero/kero_build_1.png"
                    antialiasing: true
                    fillMode: Image.PreserveAspectFit

                    Timer {
                        property int frame: 1
                        id: animationTimer
                        interval: 175
                        running: parent.visible
                        repeat: true

                        onTriggered: () => {
                            frame += 1;

                            if(frame >= 4)
                                frame = 1

                            keroLoadingImage.source = "../images/kero/kero_build_" + frame + ".png"
                        }
                    }
                }

                ColumnLayout {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Text {
                        color: palette.text
                        text: keroLoadingOverlayRoot.title
                        font: Constants.h1Font

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                    }

                    Text {
                        id: errorText
                        text: keroLoadingOverlayRoot.description
                        color: palette.text.darker()

                        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                        Layout.bottomMargin: Constants.largeMargin
                    }
                }
            }
        }
    }
}
