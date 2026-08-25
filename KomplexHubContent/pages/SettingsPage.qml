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
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHub.Plugin
import KomplexHub.Views
import KomplexHub.Pages

Item {
    readonly property bool searchable: false
    property bool settingsUpdated: wallpaperSettingsUpdated || komplexSettingsUpdated
    property bool wallpaperSettingsUpdated: wallpaperSettingsPage.updated && pageStack.currentIndex === 0
    property bool komplexSettingsUpdated: komplexSettingsPage.updated && pageStack.currentIndex === 1

    id: settingsPageRoot

    Rectangle {
        anchors.fill: parent
        color: palette.base

        ColumnLayout {
            anchors.fill: parent

            TabBar {
                id: bar

                Layout.fillHeight: false
                Layout.fillWidth: true
                TabButton {
                    text: qsTr("Wallpaper")
                }
                TabButton {
                    text: qsTr("Komplex")
                }
                TabButton {
                    text: qsTr("Account")
                }
                TabButton {
                    text: qsTr("About")
                }
            }

            StackLayout
            {
                id: pageStack
                Layout.fillHeight: true
                Layout.fillWidth: true
                currentIndex: bar.currentIndex

                Item
                {
                    id: wallpaperTab

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    WallpaperSettingsPage
                    {
                        id: wallpaperSettingsPage
                        anchors.fill: parent
                    }
                }
                Item
                {
                    id: komplexTab

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    KomplexSettingsPage
                    {
                        id: komplexSettingsPage
                        anchors.fill: parent
                    }
                }
                Item
                {
                    id: accountTab

                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    UserProfilePage
                    {
                        anchors.fill: parent
                    }
                }
                Item
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    property string title: qsTr("Hello!")
                    property string contributors: qsTr("DigitalArtifex

fridje")
                    property string thanks: qsTr("Icons8

ShaderToy

Pexels")

                    id: keroHelloAvatarRoot

                    Rectangle
                    {
                        anchors.fill: parent
                        color: "transparent"

                        ColumnLayout
                        {
                            anchors.fill: parent

                            KeroHelloAvatar
                            {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                antialiasing: true
                            }

                            Item { Layout.preferredHeight: 36 }

                            Text
                            {
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.margins: Constants.largeMargin
                                text: qsTr("Meet Kero, the small but tireless spirit behind Komplex, a curious little dragon whose name echoes the cheerful sound of a frog’s call. Hailing from the realm of **Kairoku**, Kero honed his skill in a place where unfinished ideas drift like clouds and raw creativity hums in the air.")
                                color: palette.text

                                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                                Layout.bottomMargin: Constants.largeMargin
                                wrapMode: Text.WordWrap
                                textFormat: Text.MarkdownText
                            }

                            Text
                            {
                                Layout.fillHeight: false
                                Layout.fillWidth: true
                                Layout.margins: Constants.largeMargin
                                text: qsTr("In Kairoku, landscapes aren’t painted...they’re assembled, animated, and brought to life through careful craftsmanship. Kero was once an apprentice there, learning how to shape reactive skies, breathing forests, and shimmering cityscapes. With his trusty tools and a builder’s instinct, he became known for turning even the simplest concept into something that felt alive.")
                                color: palette.text

                                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                                Layout.bottomMargin: Constants.largeMargin
                                wrapMode: Text.WordWrap
                                textFormat: Text.MarkdownText
                            }

                            Item { Layout.fillHeight: true }

                            RowLayout
                            {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignTop | Qt.AlignRight

                                ColumnLayout
                                {
                                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                                    Layout.margins: Constants.largeMargin
                                    Layout.fillHeight: true

                                    Text
                                    {
                                        text: qsTr("Contributors")
                                        color: palette.text.darker()
                                        font: Constants.h5Font

                                        Layout.alignment: Qt.AlignTop
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }

                                    Text
                                    {
                                        id: contributorText
                                        text: keroHelloAvatarRoot.contributors
                                        color: palette.text.darker()

                                        Layout.alignment: Qt.AlignBottom
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }
                                }

                                ColumnLayout
                                {
                                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                                    Layout.margins: Constants.largeMargin
                                    Layout.fillHeight: true

                                    Text
                                    {
                                        text: qsTr("Special Thanks")
                                        color: palette.text.darker()
                                        font: Constants.h5Font

                                        Layout.alignment: Qt.AlignTop
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }

                                    Text
                                    {
                                        id: thanksText
                                        text: keroHelloAvatarRoot.thanks
                                        color: palette.text.darker()

                                        Layout.alignment: Qt.AlignBottom
                                        Layout.bottomMargin: Constants.largeMargin
                                        wrapMode: Text.WordWrap
                                        textFormat: Text.MarkdownText
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Rectangle
            {
                Layout.fillWidth: true
                Layout.preferredHeight: settingsUpdated ? 64 : 0
                color: palette.base
                border.color: palette.midlight
                opacity: settingsUpdated ? 1 : 0

                RowLayout
                {
                    anchors.fill: parent
                    anchors.rightMargin: Constants.mediumMargin

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }

                    SquareButton
                    {
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: 160

                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        icon.source: "qrc:/images/icons/icons8-reply-arrow.svg"
                        text: qsTr("Reset")
                    }

                    SquareButton
                    {
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: 160
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        icon.source: "qrc:/images/icons/icons8-check-mark.svg"
                        text: qsTr("Apply")
                    }
                }

                Behavior on opacity
                {
                    NumberAnimation
                    {
                        duration: settings.animationsEnabled ?
                                      settings.normalAnimationDuration : 0
                    }
                }

                Behavior on height
                {
                    NumberAnimation
                    {
                        duration: settings.animationsEnabled ?
                                      settings.normalAnimationDuration : 0
                    }
                }
            }
        }
    }

    function applyCurrentSettings()
    {
        if(pageStack.currentIndex === 0 && wallpaperSettingsPage.updated)
        {
            wallpaperSettingsPage.apply()
        }

        else if(pageStack.currentIndex === 1 && komplexSettingsPage.updated)
        {
            komplexSettingsPage.apply()
        }
    }

    function resetCurrentSettings()
    {
        if(pageStack.currentIndex === 0 && wallpaperSettingsPage.updated)
        {
            wallpaperSettingsPage.reset()
        }

        else if(pageStack.currentIndex === 1 && komplexSettingsPage.updated)
        {
            komplexSettingsPage.reset()
        }
    }
}
