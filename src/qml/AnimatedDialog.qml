import QtQuick 2.1

FocusScope {
    id: root
    state: show ? "SHOW" : "HIDE"
    property bool show: false

    anchors.fill: parent
    opacity: 0
    visible: opacity
    scale: 0.3

    states: [
        State {
            name: "HIDE"
            PropertyChanges { target: root; opacity: 0.0}
            PropertyChanges { target: root; scale: 0.3}
        },
        State {
            name: "SHOW"
            PropertyChanges { target: root; opacity: 1.0}
            PropertyChanges { target: root; scale: 1.0}
        }
    ]

    transitions: Transition {
        id: scaleTransition
        enabled: settings.animations
        NumberAnimation { properties: "opacity,scale"; duration: 500; easing.type: Easing.InOutQuad }
    }
}
