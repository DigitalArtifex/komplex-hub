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

        delegate: Item
        {
            width: 256
            height: 245
            required property string author
            required property string description
            required property string uuid
            required property string thumbnail
            required property string authorId

            SearchResultItem
            {
                anchors.fill: parent
                title: parent.author
                description: parent.description
                thumbnail: parent.thumbnail
                uuid: parent.uuid
            }
        }
    }
}
