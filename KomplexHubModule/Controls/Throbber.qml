import QtQuick
import QtQuick.Shapes
import QtQuick.Effects
import QtQuick.Layouts

Item {
    id: throbberRoot

    onHeightChanged: () => {
        if(height > width)
        {
            layout.throbberSize = width - 24
        }
        else
        {
            layout.throbberSize = height - 24
        }
    }

    onWidthChanged: () => {
        if(height > width)
        {
            layout.throbberSize = width - 24
        }
        else
        {
            layout.throbberSize = height - 24
        }
    }

    RowLayout
    {
        property int throbberSize
        id: layout
        anchors.fill: parent
        Rectangle
        {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: parent.throbberSize
            Layout.preferredWidth: parent.throbberSize

            color: "transparent"

            Item
            {
                id: outerRing
                anchors.fill: parent

                Shape
                {
                    anchors.fill: parent
                    asynchronous: true

                    ShapePath
                    {
                        property double progress: 0
                        property int dashSpaceLength: 15
                        property int dashLength: 1

                        id: outerRingPath
                        strokeColor: palette.accent.darker(1 + (progress - 0.25))
                        fillColor: "transparent"
                        strokeWidth: 3
                        strokeStyle: ShapePath.DashLine

                        startX: 0
                        startY: outerRing.height * 0.5

                        dashPattern: [
                            dashSpaceLength - (dashSpaceLength * progress),
                            dashSpaceLength * progress
                        ]

                        PathArc
                        {
                            x: outerRing.width
                            y: outerRing.height * 0.5

                            radiusX: outerRing.width / 2
                            radiusY: outerRing.height / 2

                            useLargeArc: false
                        }

                        PathArc
                        {
                            x: 0
                            y: outerRing.height * 0.5

                            radiusX: outerRing.width / 2
                            radiusY: outerRing.height / 2

                            useLargeArc: false
                        }
                    }
                }

                transform: Rotation {
                    id: outerRingRotation
                    origin.x: outerRing.width / 2
                    origin.y: outerRing.height / 2
                }

                NumberAnimation {
                    property bool reverse: false

                    target: outerRingPath
                    property: "progress"
                    duration: 1600
                    //easing.type: Easing.InOutQuad

                    from: 0
                    to: 1
                    loops: 1
                    running: true

                    onFinished: () => {
                        if(reverse)
                        {
                            from = 0
                            to = 1
                            start()
                        }
                        else
                        {
                            from = 1
                            to = 0
                            start()
                        }

                        reverse = !reverse
                    }
                }

                NumberAnimation {
                    target: outerRingRotation
                    property: "angle"
                    duration: 16000
                    easing.type: Easing.InOutQuad

                    from: 0
                    to: (360 * 4)
                    loops: NumberAnimation.Infinite
                    running: true
                }
            }

            MultiEffect {
                source: outerRing
                anchors.fill: outerRing
                brightness: 0.4
                saturation: 0.2
                blurEnabled: true
                blurMax: 64
                blur: 1.0
            }
        }
    }
}
