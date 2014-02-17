import QtQuick 2.1
import QtMultimedia 5.0

Rectangle {
    id: root
    color: "#000"
    focus: true

    property bool hasVideo: Player.playing && Player.hasVideo

    VideoOutput {
        id: videoOutput
        visible: Player.hasVideo
        source: Player
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key !== KeyManager.keyVolume) {
            Backend.hideVideo()
        }
    }
}
