import QtQuick 2.1
import QtMultimedia 5.0
import MediaCenter 1.0

Rectangle {
    id: root
    color: "#000"
    focus: true

    property bool hasVideo: Player.state === PlayerAndModel.Playing && Player.hasVideo

    VideoOutput {
        id: videoOutput
        visible: Player.hasVideo
        source: Player
        anchors.fill: parent
    }

    Keys.onSpacePressed: Player.playing ? Player.pause() : Player.play()

    Keys.onRightPressed: {
        Player.position = Player.position + 5000
    }

    Keys.onLeftPressed: {
        Player.position = Player.position - 5000
    }
}
