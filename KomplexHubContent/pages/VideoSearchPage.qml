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

Item
{
    id: rootItem

    property string query
    readonly property bool searchable: paginator.searchable
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    VideoSearchModel
    {
        id: searchModel
        query: rootItem.query
        resultsPerPage: paginator.resultsPerPage
    }

    PaginatorGrid
    {
        id: paginator
        anchors.fill: parent
        model: searchModel
        loading: model.state === VideoSearchModel.Loading
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
                description: qsTr("No description available")
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

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: settings.animationsEnabled ?
                              settings.normalAnimationDuration : 0
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
            id: viewMoreVideoPopup
            anchors.fill: parent
            visible: opacity > 0.01
            opacity: 0
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

    function showVideoPopup(index)
    {
        searchModel.setItem(index)
        viewMoreVideoPopup.author = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorRole)
        viewMoreVideoPopup.authorUrl = searchModel.data(searchModel.index(index,0), VideoSearchModel.AuthorUrlRole)
        viewMoreVideoPopup.uuid = searchModel.data(searchModel.index(index,0), VideoSearchModel.UuidRole)
        viewMoreVideoPopup.thumbnail = searchModel.data(searchModel.index(index,0), VideoSearchModel.ThumbnailRole)
        viewMoreVideoPopup.modelIndex = index
        viewMoreVideoPopup.model = searchModel.itemModel
        viewMoreVideoPopup.opacity = 1
        paginator.opacity = 0
        popupContainer.opacity = 1
        rootItem.popup = true
    }

    function closePopup()
    {
        viewMoreVideoPopup.opacity = 0
        popupContainer.opacity = 0
        paginator.opacity = 1
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
