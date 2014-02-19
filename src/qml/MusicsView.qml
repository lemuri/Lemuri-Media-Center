import QtQuick 2.1
import MediaCenter 1.0

AnimatedDialog {
    id: root

    property string album
    property string absolutePath
    onAbsolutePathChanged: tracksModel.update(absolutePath)

    function load() {
        tracksModel.closeIndex()
        if (absolutePath.length) {
            tracksModel.getTracks(absolutePath)
        }
    }

    Frame {
        id: backgroundItem
        anchors.centerIn: parent
        width: parent.width - 20
        height: Math.min(450, parent.height - 32)
    }

    Row {
        anchors.fill: backgroundItem
        anchors.margins: 4
        spacing: 4

        Image {
            id: image
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: height
            asynchronous: true
            sourceSize.height: height
            sourceSize.width: width
            fillMode: Image.PreserveAspectCrop
            source: "image://cover/" + absolutePath
        }

        ListView {
            property string currentFilename
            id: listView
            clip: true
            interactive: false
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width - image.width
            model: TracksModel {
                id: tracksModel
                genreFilters: Backend.genreFilters
            }
            delegate: MusicDelegate {}
            onCountChanged: currentIndex = 0

            Component.onCompleted: Backend.indexChanged.connect(load)
        }
    }

    Keys.onDownPressed: {
        listView.incrementCurrentIndex()
        event.accepted = true
    }

    Keys.onUpPressed: {
        listView.decrementCurrentIndex()
        event.accepted = true
    }

    function play(space, event) {
        var currentItem = listView.currentItem
        if (space && currentItem) {
            if (!mediaPlayer.addMedia(currentItem.filePath)) {
                event.accepted = true
                return
            }
        }

        show = false
        event.accepted = true
    }

    Keys.onSpacePressed: play(true, event)
    Keys.onEnterPressed: play(false, event)
    Keys.onReturnPressed: play(false, event)
}
