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
pragma Singleton
import QtQuick
import QtQuick.Studio.Application

QtObject
{
    readonly property int width: 1920
    readonly property int height: 1080

    property string relativeFontDirectory: "fonts"

    readonly property font largeFont: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.6,
    })
    readonly property font mediumFont: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.3
    })
    readonly property font font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize
    })
    readonly property font smallFont: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 0.8
    })
    readonly property font h1Font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.8,
        bold: true
    })
    readonly property font h2Font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.6,
        bold: true
    })
    readonly property font h3Font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.4,
        bold: true
    })
    readonly property font h4Font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1.3,
        bold: true
    })
    readonly property font h5Font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 1,
        bold: true
    })
    readonly property font h6Font: Qt.font({
        family: Qt.application.font.family,
        pixelSize: Qt.application.font.pixelSize * 0.8,
        bold: true
    })

    readonly property int largeMargin: 12
    readonly property int mediumMargin: 6
    readonly property int smallMargin: 3

    readonly property int slowAnimationDuration: 750
    readonly property int normalAnimationDuration: 250
    readonly property int fastAnimationDuration: 125
}
