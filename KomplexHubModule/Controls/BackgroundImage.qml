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
import QtQuick.Effects

Item
{
    property alias source: image.source
    property alias imageOpacity: effect.opacity
    property alias blur: effect.blur
    property alias saturation: effect.saturation

    Image
    {
        anchors.fill: parent
        id: image
        source: thumbnail
        fillMode: Image.PreserveAspectFit
        visible: false
    }

    MultiEffect
    {
        id: effect
        anchors.fill: image
        source: image
        blurEnabled: true
        blurMax: 64
        blur: 1.0
        opacity: 0.25
        saturation: -0.8
    }
}
