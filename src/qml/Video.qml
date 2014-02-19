import QtQuick 2.1
import QtMultimedia 5.0
import MediaCenter 1.0

Rectangle {
    id: root
    color: "#000"

    focus: true

    property bool shouldShow: (mediaPlayer.state == 0 || mediaPlayer.state == 1) && mediaPlayer.hasVideo

    VideoOutput {
        id: videoOutput
        source: mediaPlayer
        anchors.fill: parent
    }

    Keys.onSpacePressed: mediaPlayer.state == 0 ? mediaPlayer.pause() : mediaPlayer.play()

    Keys.onEscapePressed: mediaPlayer.stop()

    Keys.onRightPressed: {
        mediaPlayer.position = mediaPlayer.position + 5000
    }

    Keys.onLeftPressed: {
        mediaPlayer.position = mediaPlayer.position - 5000
    }
}
