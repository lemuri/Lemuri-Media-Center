import QtQuick 2.1

Item {
    id: root

    property alias mediaCount: playing.count

    height: mediaCount ? playing.height + background.barAnchors.topMargin * 2 : 0
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

    Component {
        id: mediaDelegate
        Frame {
            id: root
            width: row.width + 8
            height: row.height + 8

            Row {
                id: row
                anchors.centerIn: parent
                spacing: 5

                Image {
                    id: icon
                    width: height
                    height: textItem.height
                    sourceSize.height: height
                    sourceSize.width: height
                    cache: true
                    source: index === 0 ? "image://icon/media-playback-start" : "image://icon/media-playback-pause"
                }
                Text {
                    id: textItem
                    width: contentWidth
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 20
                    color: settings.fontColorActive
                    style: Text.Raised
                    styleColor: settings.fontShadowColor
                    text: model.title
                }
            }
        }
    }

    Item {
        anchors.left: background.left
        anchors.right: background.right
        anchors.bottom: background.bottom
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        height: parent.height - background.barAnchors.topMargin

        ListView {
            id: playing
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            orientation: ListView.Horizontal
            delegate: mediaDelegate
            spacing: 5
            onCurrentItemChanged: {
                if (currentItem) {
                    playing.height = currentItem.height
                } else {
                    playing.height = 0
                }
            }
        }
    }
}
