/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex
 *  https://digitalartifex.dev - https://github.com/DigitalArtifex
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>
 */
import QtCore
import QtQuick
import QtQuick.Controls 2.0
import QtQuick.Layouts
import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubContent

Rectangle
{
    property MenuButton currentMenuButton
    property bool popup: false

    id: windowRoot

    color: palette.window

    RowLayout
    {
        id: windowRootLayout
        anchors.fill: parent
        spacing: 0

        Rectangle
        {
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

                    Behavior on opacity
                    {
                        NumberAnimation
                        {
                            duration: settings.animationsEnabled ?
                                        settings.normalAnimationDuration : 0
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
                    text: qsTr("Home")

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
                    text: qsTr("Images")

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
                    text: qsTr("Videos")

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
                    text: qsTr("Komplex")

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
                    text: qsTr("Installed")

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
                    text: qsTr("Settings")

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
                        duration: settings.animationsEnabled ? settings.normalAnimationDuration : 0
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

                    Behavior on preferredHeight
                    {
                        NumberAnimation
                        {
                            duration: settings.animationsEnabled ? settings.normalAnimationDuration : 0
                        }
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
                            duration: settings.animationsEnabled ? settings.pageChangeAnimationDuration : 0

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
                            return(
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

    Settings
    {
        id: settings
        property bool animationsEnabled: true
        //ms length of page change
        property int pageChangeAnimationDuration: 250
        property int pageFadeAnimationDuration: 250

        property int slowAnimationDuration: 750
        property int normalAnimationDuration: 250
        property int fastAnimationDuration: 125
    }
}
