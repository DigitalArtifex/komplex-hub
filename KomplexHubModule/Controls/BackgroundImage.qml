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
