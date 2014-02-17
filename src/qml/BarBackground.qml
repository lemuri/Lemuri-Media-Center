import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: root

    property alias barAnchors: highlight.anchors

    Item {
        id: bar
        visible: !dropShadow.visible
        anchors.fill: parent

        Rectangle {
            id: highlight
            smooth: true
            anchors.fill: parent

            gradient: Gradient {
                GradientStop { position: 0.0; color: "#101010" }
                GradientStop { position: 0.3; color: "#404040" }
                GradientStop { position: 1.0; color: "#090909" }
            }
        }
    }

    DropShadow {
        id: dropShadow
        anchors.fill: source
        source: bar
        radius: 8
        samples: 8
        cached: true
        spread: 0.2
        color: "#000"
    }
}
