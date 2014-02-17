import QtQuick 2.1
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0

AnimatedDialog {
    id: root

    Frame {
        id: backgroundItem
        anchors.fill: volumeRow
        anchors.rightMargin: -4
        anchors.leftMargin: -4
    }

    RowLayout {
        id: volumeRow
        anchors.centerIn: parent

        Text {
            id: volumeText
            Layout.minimumHeight: height
            color: settings.fontColorActive
            style: Text.Raised
            styleColor: settings.fontShadowColor
            font.pointSize: settings.fontM
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: "Volume"
        }

        ProgressBar {
            id: volumeBar
            focus: true
            opacity: 0.9
            height: volumeText.contentHeight
            maximumValue: 100
            value: Backend.volume

            Keys.onPressed: {
                if (event.key === Qt.Key_Left || event.key === Qt.Key_Down) {
                    Backend.decreaseVolume()
                    event.accepted = true
                } else if (event.key === Qt.Key_Right || event.key === Qt.Key_Up) {
                    Backend.increaseVolume()
                    event.accepted = true
                }
            }
        }
    }
}
