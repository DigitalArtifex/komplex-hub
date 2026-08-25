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

    property string query
    readonly property bool searchable: paginator.searchable
    property bool popup: false
    property int resultWidth: 256
    property int resultHeight: 245

    color: palette.base

    FeaturedImagesModel
    {
        id: searchModel
        resultsPerPage: paginator.resultsPerPage

        Component.onCompleted: () => nextPage()
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
            title: qsTr("Featured Images")
            description: qsTr("A specially curated collection of images, courtesy of Pexels")
        }

        PaginatorGrid
        {
            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true

            id: paginator
            model: searchModel
            loading: model.state === FeaturedImagesModel.Loading

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
    }

    Rectangle
    {
        id: popupContainer
        anchors.fill: parent
        opacity: 0
        visible: opacity > 0.01
        color: palette.base

        ImageView
        {
            id: viewMoreImagePopup
            anchors.fill: parent
            opacity: 0
            visible: opacity > 0.01

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
        viewMoreImagePopup.author = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.AuthorRole)
        viewMoreImagePopup.authorUrl = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.AuthorUrlRole)
        viewMoreImagePopup.description = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.DescriptionRole)
        viewMoreImagePopup.uuid = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.UuidRole)
        viewMoreImagePopup.thumbnail = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LargeThumbnailRole)
        viewMoreImagePopup.original = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.OriginalUrlRole)
        viewMoreImagePopup.portrait = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.PortraitUrlRole)
        viewMoreImagePopup.landscape = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LandscapeUrlRole)
        viewMoreImagePopup.backgroundPortrait = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.BackgroundPortraitRole)
        viewMoreImagePopup.backgroundLandscape = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.BackgroundLandscapeRole)
        viewMoreImagePopup.fullScreen = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ScreenUrlRole)
        viewMoreImagePopup.portraitSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.PortraitSizeRole)
        viewMoreImagePopup.landscapeSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LandscapeSizeRole)
        viewMoreImagePopup.fullScreenSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ScreenSizeRole)
        viewMoreImagePopup.opacity = 1
        popupContainer.opacity = 1
        resultsLayout.opacity = 0
        rootItem.popup = true
    }

    function closePopup()
    {
        viewMoreImagePopup.opacity = 0
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
