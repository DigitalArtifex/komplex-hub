import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import KomplexHub
import KomplexHub.Controls
import KomplexHub.Kero
import KomplexHubPlugin

Item
{
    id: rootItem

    property string query
    readonly property bool searchable: paginator.searchable
    property bool popup: false

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

            width: 256
            height: 245

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
                    duration: 250
                }
            }
        }

        Behavior on opacity {
            NumberAnimation
            {
                duration: 250
            }
        }
    }

    function showImagePopup(index)
    {
        viewMoreImagePopup.author = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.AuthorRole)
        viewMoreImagePopup.authorUrl = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.AuthorUrlRole)
        viewMoreImagePopup.description = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.DescriptionRole)
        viewMoreImagePopup.uuid = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.UuidRole)
        viewMoreImagePopup.thumbnail = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ThumbnailRole)
        viewMoreImagePopup.small = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.SmallUrlRole)
        viewMoreImagePopup.medium = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.MediumUrlRole)
        viewMoreImagePopup.large = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LargeUrlRole)
        viewMoreImagePopup.extraLarge = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ExtraLargeUrlRole)
        viewMoreImagePopup.original = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.OriginalUrlRole)
        viewMoreImagePopup.portrait = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.PortraitUrlRole)
        viewMoreImagePopup.landscape = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LandscapeUrlRole)
        viewMoreImagePopup.smallSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.SmallSizeRole)
        viewMoreImagePopup.mediumSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.MediumSizeRole)
        viewMoreImagePopup.largeSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LargeSizeRole)
        viewMoreImagePopup.extraLargeSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.ExtraLargeSizeRole)
        viewMoreImagePopup.originalSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.OriginalSizeRole)
        viewMoreImagePopup.portraitSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.PortraitSizeRole)
        viewMoreImagePopup.landscapeSize = searchModel.data(searchModel.index(index,0), FeaturedImagesModel.LandscapeSizeRole)
        viewMoreImagePopup.opacity = 1
        popupContainer.opacity = 1
        rootItem.popup = true
    }

    function closePopup()
    {
        viewMoreImagePopup.opacity = 0
        popupContainer.opacity = 0
        rootItem.popup = false
    }
}
