import QtQuick 2.1

FocusScope {
    id: root

    state: mediaView.shouldShow ? "VIDEO" : "MEDIA"

    Media {
        id: mediaSelector
        visible: false
        anchors.fill: parent
    }

    Video {
        id: mediaView
        visible: false
        anchors.fill: parent
    }

    states: [
        State {
            name: "MEDIA"
            PropertyChanges { target: mediaSelector; visible: true}
            PropertyChanges { target: mediaSelector; focus: true}
        },
        State {
            name: "VIDEO"
            PropertyChanges { target: mediaView; visible: true}
            PropertyChanges { target: mediaView; focus: true}
        }
    ]
}
