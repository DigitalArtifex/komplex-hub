/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls 2.0
import QtQuick.Layouts
import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubContent

Rectangle {
    property MenuButton currentMenuButton
    property bool popup: false

    id: windowRoot

    color: palette.window

    RowLayout {
        id: windowRootLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: windowMenu

            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.preferredWidth: 100
            Layout.minimumWidth: 100
            Layout.maximumWidth: 100

            color: palette.base.lighter()

            ColumnLayout
            {
                height: popup ? 64 : 0
                opacity: popup ? 1 : 0

                MenuButton
                {
                    Layout.alignment: Qt.AlignTop

                    height: 64
                    width: 100
                    text: qsTr("Back")

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-reply-arrow.svg"

                    onTriggered: () => {
                        pageLoader.item.closePopup()
                        selected = false
                    }

                    Behavior on opacity {
                        NumberAnimation
                        {
                            duration: 250
                        }
                    }
                }
            }

            ColumnLayout
            {
                opacity: popup ? 0 : 1
                visible: opacity > 0.01
                id: windowMenuLayout
                Layout.fillHeight: true
                anchors.fill: parent

                MenuButton {
                    id: homeMenubutton
                    Layout.alignment: Qt.AlignTop

                    height: 64
                    width: 100
                    text: "Home"

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-famous.svg"

                    onTriggered: () =>
                    {
                        pageLoader.page = "pages/HomePage.qml"

                        if(currentMenuButton !== null)
                        {
                            currentMenuButton.selected = false
                        }

                        currentMenuButton = this

                        searchContainer.preferredHeight = 0
                        searchContainer.reset()
                    }
                }

                MenuButton
                {
                    Layout.alignment: Qt.AlignTop

                    height: 64
                    width: 100
                    text: "Images"

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-image.svg"

                    onTriggered: () => {
                        pageLoader.page = "pages/ImageSearchPage.qml"

                        currentMenuButton.selected = false
                        currentMenuButton = this

                        searchContainer.preferredHeight = 50
                        searchContainer.reset()
                    }
                }

                MenuButton
                {
                    Layout.alignment: Qt.AlignTop

                    height: 64
                    width: 100
                    text: "Videos"

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-video.svg"

                    onTriggered: () => {
                        pageLoader.page = "pages/VideoSearchPage.qml"

                        currentMenuButton.selected = false
                        currentMenuButton = this

                        searchContainer.preferredHeight = 50
                        searchContainer.reset()
                    }
                }

                MenuButton
                {
                    Layout.alignment: Qt.AlignTop

                    height: 64
                    width: 100
                    text: "Live"

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-biotech.svg"

                    onTriggered: () => {
                        pageLoader.page = "pages/LiveSearchPage.qml"

                        currentMenuButton.selected = false
                        currentMenuButton = this

                        searchContainer.preferredHeight = 50
                        searchContainer.reset()
                    }
                }

                MenuButton
                {
                    Layout.alignment: Qt.AlignTop

                    height: 64
                    width: 100
                    text: "Installed"

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-hdd.svg"

                    onTriggered: () => {
                        pageLoader.page = "pages/InstalledWallpaperPage.qml"

                        currentMenuButton.selected = false
                        currentMenuButton = this

                        searchContainer.preferredHeight = 0
                        searchContainer.reset()
                    }
                }

                Item
                {
                    Layout.fillHeight: true
                    Layout.minimumHeight: 100
                }

                MenuButton
                {
                    Layout.alignment: Qt.AlignBottom

                    height: 64
                    width: 100
                    text: "Settings"

                    icon.height: 32
                    icon.width: 32
                    icon.url: "qrc:/images/icons/icons8-settings.svg"

                    onTriggered: () => {
                        pageLoader.page = "pages/SettingsPage.qml"

                        currentMenuButton.selected = false
                        currentMenuButton = this

                        searchContainer.preferredHeight = 0
                        searchContainer.reset()
                    }
                }

                Behavior on opacity
                {
                    NumberAnimation
                    {
                        duration: 250
                    }
                }
            }
        }

        Rectangle
        {
            id: windowContainer

            Layout.fillHeight: true
            Layout.fillWidth: true

            color: "transparent"

            ColumnLayout
            {
                id: windowContainerLayout
                anchors.fill: parent
                spacing: 0

                SearchBar
                {
                    property int preferredHeight: 50
                    id: searchContainer
                    color: palette.base.lighter()

                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    Layout.preferredHeight: preferredHeight

                    icon: "qrc:/images/icons/icons8-search.svg"

                    onSearchTermChanged: () =>
                    {
                        pageLoader.setSearchTerm()
                    }

                    Behavior on preferredHeight {
                        NumberAnimation { duration: Constants.normalAnimationDuration }
                    }
                }

                Rectangle
                {
                    id: windowContent

                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.leftMargin: 0
                    Layout.topMargin: 0

                    color: palette.base.darker()

                    Loader
                    {
                        property bool loading: false
                        property string page

                        anchors.fill: parent

                        onPageChanged: () =>
                        {
                            loading = true
                            fadeOutPage()
                        }

                        id: pageLoader

                        OpacityAnimator on opacity
                        {
                            id: pageLoaderAnimation

                            target: pageLoader
                            duration: 150

                            onFinished: () => {
                                if(pageLoader.loading && pageLoader.opacity === 0)
                                {
                                    pageLoader.loadPage()
                                }
                                else if(pageLoader.loading && pageLoader.opacity === 1)
                                {
                                    pageLoader.loading = false
                                    pageLoader.updateSearchTerm();
                                    popupConnection.target = pageLoader.item
                                }
                            }
                        }

                        function fadeOutPage()
                        {
                            pageLoaderAnimation.from = 1
                            pageLoaderAnimation.to = 0
                            pageLoaderAnimation.start()
                        }

                        function fadeInPage()
                        {
                            if(pageLoader.sourceComponent.status != Component.Ready)
                            {
                                return
                            }

                            pageLoaderAnimation.from = 0
                            pageLoaderAnimation.to = 1
                            pageLoaderAnimation.start()
                        }

                        function loadPage()
                        {
                            sourceComponent = Qt.createComponent(page)

                            if(sourceComponent.status == Component.Ready)
                            {
                                fadeInPage()
                            }
                            else
                            {
                                statusChanged.connect(fadeInPage)
                            }
                        }

                        function updateSearchTerm()
                        {
                            if(isSearchPage())
                            {
                                searchContainer.searchTerm = pageLoader.item.query
                            }
                        }

                        function setSearchTerm()
                        {
                            if(isSearchPage())
                            {
                                pageLoader.item.query = searchContainer.searchTerm
                            }
                        }

                        function isSearchPage()
                        {
                            return (
                                pageLoader.item instanceof ImageSearchPage ||
                                pageLoader.item instanceof LiveSearchPage  ||
                                pageLoader.item instanceof VideoSearchPage
                            );
                        }
                    }
                }
            }
        }
    }

    KeroBuildingAnimation
    {
        id: buildingOverlay

        anchors.fill: parent
        visible: false
    }

    KeroLoadingAnimation
    {
        id: loadingAnimation
        anchors.fill: parent

        visible: opacity > 0
        opacity: 0
    }

    Connections
    {
        id: popupConnection
        target: pageLoader.item

        function onPopupChanged()
        {
            windowRoot.popup = pageLoader.item.popup
        }
    }

    Component.onCompleted: () => {
        homeMenubutton.selected = true
        currentMenuButton = homeMenubutton
        pageLoader.page = "pages/HomePage.qml"
        searchContainer.preferredHeight = 0
        loadingAnimation.opacity = 0
    }

    states: [
        State {
            name: "build_error"
            PropertyChanges {
                target: windowOverlay
                visible: true
            }
        },
        State {
            name: "building"
            PropertyChanges {
                target: buildingOverlay
                visible: true
            }
        },
        State {
            name: "build_complete"
        },
        State
        {
            name: "popup"
            when: popup
            PropertyChanges {
                target: searchContainer
                preferredHeight: 0
            }
        }

    ]
}
