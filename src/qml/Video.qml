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

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            Backend.showCursor = true
            hideCursorTimer.start()
        }
        onDoubleClicked: Backend.fullScreen = !Backend.fullScreen
    }

    PlayingBar {
        id: playingBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        visible: Backend.showCursor
        onHoveredChanged: {
            if (hovered) {
                hideCursorTimer.stop()
            } else {
                hideCursorTimer.start()
            }
        }
    }

    Timer {
        id: hideCursorTimer
        interval: 5000
        onTriggered: Backend.showCursor = false
    }

    Keys.onSpacePressed: mediaPlayer.state == 0 ? mediaPlayer.pause() : mediaPlayer.play()

    Keys.onEscapePressed: mediaPlayer.stop()

    Keys.onRightPressed: {
        console.debug("RIGHT pos1: " + mediaPlayer.position)
        mediaPlayer.position = mediaPlayer.position + 5000
        console.debug("RIGHT pos2: " + mediaPlayer.position)
    }

    Keys.onLeftPressed: {
        mediaPlayer.position = mediaPlayer.position - 5000
    }
}
