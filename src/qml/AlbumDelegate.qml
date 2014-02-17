import QtQuick 2.1

Item {
    id: root

    property bool isSelected: root.GridView.isCurrentItem
    property bool imageReady: cover.status === Image.Ready
    property int shadow: isSelected ? 12 : 8

    property string absolutePath: rolePath
    property string album: roleName
    property string artist: roleArtist
    property string coverPath: roleCover
    onCoverPathChanged: {
        if (coverPath.length) {
            cover.source = coverPath
        }
    }

    scale: isSelected ? 1.1 : 0.9
    z: isSelected ? 1 : 0

    Behavior on scale {
        enabled: settings.animations
        NumberAnimation { duration: 700; easing.type: Easing.OutElastic }
    }

    BorderImage {
        visible: imageReady
        anchors.fill: cover
        anchors.margins: -shadow
        border.left: shadow
        border.right: shadow
        border.top: shadow
        border.bottom: shadow
        smooth: true
        source: isSelected ? "qrc:/images/box-shadow-selected.png" : "qrc:/images/box-shadow.png"
    }

    Image {
        id: cover
        smooth: true
        anchors.fill: parent
        anchors.margins: shadow * 0.7
        sourceSize.height: parent.height
        sourceSize.width: parent.width
        fillMode: Image.PreserveAspectCrop
        asynchronous: true

        Image {
            opacity: 0.7
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: parent.width / 3
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            source: roleDvd ? "qrc:/images/dvd.png" : ""
        }
    }

    Image {
        id: cd
        visible: !imageReady
        anchors.fill: parent
        anchors.margins: shadow * 0.7
        sourceSize.height: height
        sourceSize.width: width
        asynchronous: true
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images/cd.png"
    }
}
