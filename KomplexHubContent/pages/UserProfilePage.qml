import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import KomplexHub

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true

            Image {
                Layout.preferredHeight: 128
                Layout.preferredWidth: 128
                id: userAccountImage
                source: "qrc:/images/icons/icons8-user-account.svg"
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: Constants.largeMargin
                Layout.leftMargin: Constants.largeMargin
                Layout.bottomMargin: Constants.largeMargin
                Layout.rightMargin: Constants.largeMargin
                Layout.fillWidth: true
                Layout.fillHeight: true

                Text {
                    text: qsTr("Username")
                    font.pixelSize: Constants.largeFont.pixelSize
                    font.bold: true
                    color: palette.text
                }

                Text {
                    text: qsTr("Thursday, April 30th 2026")
                    font.pixelSize: Constants.mediumMargin.pixelSize
                    color: palette.text
                }

                RowLayout {
                    Text {
                        text: qsTr("Wallpapers Submitted:")
                        font.pixelSize: Constants.largeFont.pixelSize
                        color: palette.text
                    }
                    Text {
                        text: qsTr("12")
                        font.pixelSize: Constants.largeFont.pixelSize
                        font.bold: true
                        color: palette.text
                    }
                }
            }
        }
    }
}
