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

    ImageSearchModel
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
        loading: model.state === ImageSearchModel.Loading

        delegate: ItemDelegate
        {
            id: resultDelegate

            width: resultWidth
            height: resultHeight

            required property string author
            required property string description
            required property string uuid
            required property string thumbnail
            required property string authorId
            required property int index

            SearchResultItem
            {
                anchors.fill: parent
                title: parent.author
                description: parent.description
                thumbnail: parent.thumbnail
                uuid: parent.uuid
                pexels: true
                selected: paginator.currentIndex === parent.index

                onTriggered: () => paginator.currentIndex = parent.index

                onViewMoreTriggered: () =>
                {
                    showImagePopup(parent.index)
                }
            }
        }
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        color: palette.base

        ImageView
        {
            id: viewMoreImagePopup
            anchors.fill: parent
            opacity: 0

            Behavior on opacity {
                NumberAnimation
                {
                    duration: settings.animationsEnabled ?
                                  settings.normalAnimationDuration : 0
                }
            }
        }

        Behavior on opacity {
            NumberAnimation
            {
                duration: settings.animationsEnabled ?
                              settings.normalAnimationDuration : 0
            }
        }
    }

    function showImagePopup(index)
    {
        viewMoreImagePopup.author = searchModel.data(searchModel.index(index,0), ImageSearchModel.AuthorRole)
        viewMoreImagePopup.authorUrl = searchModel.data(searchModel.index(index,0), ImageSearchModel.AuthorUrlRole)
        viewMoreImagePopup.description = searchModel.data(searchModel.index(index,0), ImageSearchModel.DescriptionRole)
        viewMoreImagePopup.uuid = searchModel.data(searchModel.index(index,0), ImageSearchModel.UuidRole)
        viewMoreImagePopup.thumbnail = searchModel.data(searchModel.index(index,0), ImageSearchModel.LargeThumbnailRole)
        viewMoreImagePopup.original = searchModel.data(searchModel.index(index,0), ImageSearchModel.OriginalUrlRole)
        viewMoreImagePopup.portrait = searchModel.data(searchModel.index(index,0), ImageSearchModel.PortraitUrlRole)
        viewMoreImagePopup.landscape = searchModel.data(searchModel.index(index,0), ImageSearchModel.LandscapeUrlRole)
        viewMoreImagePopup.backgroundPortrait = searchModel.data(searchModel.index(index,0), ImageSearchModel.BackgroundPortraitRole)
        viewMoreImagePopup.backgroundLandscape = searchModel.data(searchModel.index(index,0), ImageSearchModel.BackgroundLandscapeRole)
        viewMoreImagePopup.fullScreen = searchModel.data(searchModel.index(index,0), ImageSearchModel.ScreenUrlRole)
        viewMoreImagePopup.portraitSize = searchModel.data(searchModel.index(index,0), ImageSearchModel.PortraitSizeRole)
        viewMoreImagePopup.landscapeSize = searchModel.data(searchModel.index(index,0), ImageSearchModel.LandscapeSizeRole)
        viewMoreImagePopup.fullScreenSize = searchModel.data(searchModel.index(index,0), ImageSearchModel.ScreenSizeRole)
        viewMoreImagePopup.opacity = 1
        popupContainer.opacity = 1
        paginator.opacity = 0
        rootItem.popup = true
    }

    function closePopup()
    {
        viewMoreImagePopup.opacity = 0
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
