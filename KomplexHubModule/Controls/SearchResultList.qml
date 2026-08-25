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

Item {
    readonly property bool searchable: true
    property int resultsPerPage: resultsPerRow * resultsPerColumn
    property int resultsPerRow: width / (256 + Constants.largeMargin)
    property int resultsPerColumn: height / (256 + Constants.largeMargin)
    property int totalResults: 7
    property int currentPage: 1
    property color color: "transparent"

    id: searchResultsPageRoot

    Rectangle {
        anchors.fill: parent
        color: searchResultsPageRoot.color

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Constants.largeMargin

            Component {
                id: highlight

                Rectangle {
                    width: view.cellWidth; height: view.cellHeight
                    color: "lightsteelblue";
                    radius: 5
                    x: view.currentItem.x
                    y: view.currentItem.y
                    Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                    Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
                }
            }

            //So the idea here is that the real query would request X number of results
            // (say 50), but then the cache aggregator will paginate on the cache
            // in a way that it would also account for the total number of results for
            // the search term

            //this is dummy data for the cache
            WallpaperModelData {
                property int totalResults: 17
                property string term: ""

                onTermChanged: () => {
                    //clear()

                    //this will be in the plugin
                    //query(0, 100, term)
                }

                id: wallpaperDataCache

                //use the current search term to get the next chunk of data from the server
                function extendCache() {
                    //this will be in the plugin
                    //query(count, 100, term)
                }
            }

            ListModel {
                property int index: 0

                id: wallpaperCacheAggregator

                //get the previous page data
                function previous() {
                    searchResultsPageRoot.state = "loading"
                    index -= searchResultsPageRoot.resultsPerPage

                    if(index < 0)
                        index = 0

                    searchResultsPageRoot.currentPage -= 1

                    if(searchResultsPageRoot.currentPage < 1)
                        searchResultsPageRoot.currentPage = 1

                    searchResultsPageRoot.state = ""
                    aggregateCache()
                }

                //get the next page data
                function next() {
                    searchResultsPageRoot.state = "loading"

                    index += searchResultsPageRoot.resultsPerPage

                    if(index >= searchResultsPageRoot.totalResults)
                        index = searchResultsPageRoot.totalResults - searchResultsPageRoot.resultsPerPage

                    searchResultsPageRoot.currentPage += 1

                    if(searchResultsPageRoot.currentPage > Math.ceil(searchResultsPageRoot.totalResults / searchResultsPageRoot.resultsPerPage))
                        searchResultsPageRoot.currentPage = Math.ceil(searchResultsPageRoot.totalResults / searchResultsPageRoot.resultsPerPage)

                    //download next chunk of data if we're at the end
                    if(index >= wallpaperDataCache.count)
                        wallpaperDataCache.extendCache()

                    searchResultsPageRoot.state = ""
                    aggregateCache()
                }

                //re-aggregate the current cache
                function aggregateCache() {
                    clear()

                    for(var i = 0; i < searchResultsPageRoot.resultsPerPage && (index + i) < wallpaperDataCache.count; i++)
                        insert(i, wallpaperDataCache.get(index + i))
                }
            }

            GridView {
                id: resultsView
                Layout.fillHeight: true
                Layout.fillWidth: true
                cellWidth: 256 + Constants.mediumMargin
                cellHeight: 256 + Constants.mediumMargin
                clip: true
                model: wallpaperCacheAggregator

                delegate: Rectangle {
                    required property string author
                    required property string description
                    required property string name
                    required property string thumbnail
                    required property string url
                    required property string uuid

                    SearchResultItem {
                        author: parent.author
                        description: parent.description
                        title: parent.name
                        thumbnail: parent.thumbnail
                        url: parent.url
                        uuid: parent.uuid
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: false
                Layout.preferredHeight: 36
                Layout.margins: Constants.largeMargin

                SquareButton {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 160

                    icon.source: "qrc:/images/icons/icons8-back.svg"
                    icon.height: 16
                    icon.width: 16
                    text: qsTr("Previous")

                    enabled: searchResultsPageRoot.currentPage > 1

                    onTriggered: () => {
                        wallpaperCacheAggregator.previous()
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                SquareButton {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 160

                    icon.source: "qrc:/images/icons/icons8-next.svg"
                    icon.height: 16
                    icon.width: 16
                    text: qsTr("Next")

                    enabled: searchResultsPageRoot.currentPage < Math.ceil(searchResultsPageRoot.totalResults / searchResultsPageRoot.resultsPerPage)

                    onTriggered: () => {
                        wallpaperCacheAggregator.next()
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "loading"
        },
        State {
            name: "empty"
        },
        State {
            name: "error"
        }
    ]

    onResultsPerPageChanged: () => wallpaperCacheAggregator.aggregateCache();
    Component.onCompleted: () => wallpaperCacheAggregator.aggregateCache()

}
