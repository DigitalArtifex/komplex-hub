import QtQuick
import QtQuick.Controls 2.15
import KomplexHub
import org.kde.kirigami as Kirigami

ApplicationWindow {

    visible: true
    flags: Qt.Window
    title: "KomplexHub"
    color: "transparent"
    minimumWidth: 720

    MainScreen {
        antialiasing: true
        id: mainScreen

        anchors.fill: parent
    }
}

