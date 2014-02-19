import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Item {
    id: root

    property bool updatingSlider: false
    property int auxValue
    property bool hovered: slider.hovered || playPauseArea.containsMouse || stopArea.containsMouse

    height: visible ? actionRow.height + background.barAnchors.topMargin * 2 : 0
    Behavior on height {
        enabled: settings.animations
        NumberAnimation { duration: 700; easing.type: Easing.OutExpo }
    }

    BarBackground {
        id: background
        opacity: 0.5
        anchors.fill: parent
        barAnchors.bottomMargin: 10
    }

    Item {
        anchors.left: background.left
        anchors.right: background.right
        anchors.bottom: background.bottom
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        height: parent.height - background.barAnchors.topMargin

        RowLayout {
            id: actionRow
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            Image {
                id: stop
                width: height
                height: 64
                sourceSize.height: height
                sourceSize.width: height
                cache: true
                source: "image://icon/media-playback-stop"
                MouseArea {
                    id: stopArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: mediaPlayer.stop()
                }
            }
            Image {
                id: playPauseIcon
                width: height
                height: 64
                sourceSize.height: height
                sourceSize.width: height
                cache: true
                source: mediaPlayer.state === 0 ? "image://icon/media-playback-pause" : "image://icon/media-playback-start"
                MouseArea {
                    id: playPauseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: mediaPlayer.state === 0 ? mediaPlayer.pause() : mediaPlayer.play()
                }
            }
            Slider {
                id: slider
                Layout.fillWidth: true
                Layout.columnSpan: 2
                maximumValue: mediaPlayer.duration / 1000
                stepSize: 5
                onValueChanged: {
                    // Make sure we don't change the pos if we are updating it
                    if (!updatingSlider) {
                        mediaPlayer.position = value * 1000
                    }
                }
                Component.onCompleted: {
                    minimumValue = 100
                }
                Connections {
                    target: mediaPlayer
                    onPositionChanged: {
                        auxValue = mediaPlayer.position / 1000
                        if (auxValue !== slider.value) {
                            updatingSlider = true
                            slider.value = auxValue
                            updatingSlider = false
                        }
                    }
                }
            }
        }
    }
}
