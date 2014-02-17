import QtQuick 2.1

Item {
    id: root

    Rectangle {
        id: backgroundItem
        anchors.centerIn: parent
        width: parent.width
        height: list.contentHeight
        border.color: "#808080"
        border.width: 1
        opacity: 0.9
        color: "#404040"
        Behavior on height {
            NumberAnimation { duration: 500; easing.type: Easing.OutExpo }
        }
    }

    ListModel {
        id: notifications
    }

    function notify(message) {
        notifications.append({"label": message})
    }

    ListView {
        id: list
        clip: true
        anchors.top: backgroundItem.top
        anchors.left: backgroundItem.left
        anchors.right: backgroundItem.right
        anchors.bottom: backgroundItem.bottom
        model: notifications
        spacing: 5
        delegate: Text {
            width: ListView.view.width
            font.pointSize: settings.fontS
            color: settings.fontColorActive
            style: Text.Raised
            styleColor: settings.fontShadowColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            text: model.label

            Timer {
                id: cleanup
                interval: 3000
                running: root.visible
                onTriggered: {
                    notifications.remove(index)
                }
            }
        }
    }

    Component.onCompleted: {
        Backend.notification.connect(notify)
    }
}
