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

                selected: resultDelegate.highlighted
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked: paginator.currentIndex = index
            }
        }
    }
}
