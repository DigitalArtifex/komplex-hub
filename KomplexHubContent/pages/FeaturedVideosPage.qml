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

Rectangle
{
    id: rootItem

    readonly property bool searchable: false
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    color: palette.base

    FeaturedVideosModel
    {
        id: videosModel
        resultsPerPage: paginator.resultsPerPage
        windowSize: 80
    }

    ColumnLayout
    {
        id: resultsLayout
        visible: opacity > 0.01
        anchors.fill: parent
        anchors.margins: Constants.mediumMargin

        KeroHeader
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: 180
            title: qsTr("Most Popular Videos")
            description: qsTr("Most popular videos, provided courtesy of Pexels")
            image: "qrc:/images/kero/kero_star.png"
        }

        PaginatorGrid
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: paginator
            model: videosModel
            loading: videosModel.state === FeaturedVideosModel.Loading
            visible: opacity > 0.01

            delegate: ItemDelegate
            {
                id: resultDelegate

                width: resultWidth
                height: resultHeight

                required property string uuid
                required property string author
                required property string authorId
                required property string authorUrl
                required property string thumbnail
                required property int index

                SearchResultItem
                {
                    anchors.fill: parent
                    author: parent.author
                    description: qsTr("")
                    thumbnail: parent.thumbnail
                    uuid: parent.uuid
                    pexels: true
                    selected: paginator.currentIndex === parent.index

                    onTriggered: () => paginator.currentIndex = parent.index

                    onViewMoreTriggered: () =>
                    {
                        showVideoPopup(parent.index)
                    }
                }
            }
        }
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        visible: opacity > 0.01
        color: palette.base

        VideoView
        {
            id: videoDetailsPopup
            anchors.fill: parent
            opacity: 0
            visible: opacity > 0.01

            Behavior on opacity
            {
                NumberAnimation
                {
                    duration: settings.animationsEnabled ?
                                  settings.normalAnimationDuration : 0
                }
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
    }

    Component.onCompleted: () =>
    {
        videosModel.nextPage()
    }

    function showVideoPopup(index)
    {
        videosModel.setItem(index)
        videoDetailsPopup.author = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorRole)
        videoDetailsPopup.authorUrl = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorUrlRole)
        videoDetailsPopup.uuid = searchModel.data(searchModel.index(index,0), VideoSearchModel.UuidRole)
        videoDetailsPopup.thumbnail = searchModel.data(searchModel.index(index,0), VideoSearchModel.ThumbnailRole)
        videoDetailsPopup.modelIndex = index
        videoDetailsPopup.model = searchModel.itemModel
        videoDetailsPopup.opacity = 1
        resultsLayout.opacity = 0
        popupContainer.opacity = 1
        rootItem.popup = true
    }

    function closePopup()
    {
        videoDetailsPopup.opacity = 0
        popupContainer.opacity = 0
        resultsLayout.opacity = 1
        rootItem.popup = false
    }

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
