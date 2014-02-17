import QtQuick 2.1

Item {
    id: root

    property bool isSelected: ListView.isCurrentItem
    property string filePath: roleFilePath
    property string title: roleTitle
    property string artist: roleArtist

    width: ListView.view.width - 4
    height: textItem.height + 4

    Behavior on scale {
        enabled: settings.animations
        NumberAnimation { duration: 500; easing.type: Easing.OutElastic }
    }

    Rectangle {
        opacity: root.isSelected ? 0.8 : 0
        radius: 5
        anchors.fill: parent
        color: settings.selectionColor
    }

    Image {
        id: icon
        width: height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 2
        fillMode: Image.PreserveAspectFit
        source: roleIsVideo ? "image://icon/tool-animator" : "image://icon/media-optical-audio"
    }

    Text {
        id: textItem
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: icon.right
        anchors.leftMargin: 3
        width: parent.width - icon.width - anchors.leftMargin
        elide: Text.ElideRight
        color: isSelected ? settings.fontShadowColor : settings.fontColorActive
        style: Text.Raised
        styleColor: isSelected ? settings.fontColorActive : settings.fontShadowColor
        font.pointSize: settings.fontS
        text: roleTitle
    }
}
