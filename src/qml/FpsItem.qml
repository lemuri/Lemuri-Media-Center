import QtQuick 2.1

Item {
    id: root
    property int frameCounter: 0
    property int fps: 0;

    width: 160
    height: 48

    Image {
        id: spinnerImage
        source: "images/spinner.png"
        NumberAnimation on rotation {
            from:0
            to: 360
            duration: 800
            running: root.visible
            loops: Animation.Infinite
        }
        onRotationChanged: frameCounter++;
    }

    Text {
        anchors.right: parent.right
        anchors.verticalCenter: spinnerImage.verticalCenter
        color: "#ffffff"
        style: Text.Outline
        styleColor: "#606060"
        font.pixelSize: 28
        text: root.fps + " fps"
        onTextChanged: console.debug(text)
    }

    Timer {
        interval: 2000
        repeat: true
        running: root.visible
        onTriggered: {
            fps = frameCounter/2;
            frameCounter = 0;
        }
    }
}
