import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: root

    // Set this to blur the mainView when showing something on top of it
    property real blurAmount: 80
    property string source
    property string _source

    state: settings.coverBackground
    onStateChanged: update()

    onSourceChanged: {
        if (state === "COVER") {
             updaterTimer.restart()
        }
    }

    function update() {
        if (state === "COVER") {
            _source = "image://cover/" + source
            backgroundImage.source = _source
        } else if (state === "DIRECTORY") {
            if (backgroundImage.currentItem === BackgroundModel.count -1) {
                backgroundImage.currentItem = 0
            }

            if (backgroundImage.currentItem < BackgroundModel.count) {
                _source = "file://" + BackgroundModel.get(backgroundImage.currentItem)
                backgroundImage.source = _source
                backgroundImage.currentItem++
            }
        }
    }

    Rectangle {
        id: plainColor
        anchors.fill: parent
        visible: false
        color: settings.plainBackgroundColor
    }

    RadialGradient {
        id: radial
        anchors.fill: parent
        visible: false
        cached: true
        gradient: Gradient {
            GradientStop { position: 0.0
                color: "#00000000"
            }
            GradientStop { position: 1.0
                color: settings.radialColor
            }
        }
    }

    Item {
        id: background
        anchors.fill: parent
        visible: false

        Image {
            id: buffer
            anchors.fill: parent
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            sourceSize.height: parent.height
            sourceSize.width: parent.width
        }

        Image {
            id: backgroundImage
            anchors.fill: parent
            asynchronous: true
            fillMode: Image.PreserveAspectCrop
            sourceSize.height: parent.height
            sourceSize.width: parent.width

            property int currentItem: 0

            Behavior on source {
                enabled: settings.coverBackground
                SequentialAnimation {
                    PropertyAction { target: buffer; property: "source"; value: _source }
                    NumberAnimation { target: backgroundImage; property: "opacity"; to: 0; duration: 500; easing.type: Easing.InCurve }
                    PropertyAction { target: backgroundImage; property: "source" }
                    PropertyAction { target: backgroundImage; property: "opacity"; value: 1 }
                }
            }
        }
    }

    Timer {
        id: updaterTimer
        interval: repeat ? 15000 : 500
        running: backgroundImage.visible
        onTriggered: update()
    }

    FastBlur {
        id: fastBlur
        visible: false
        anchors.fill: parent
        radius: blurAmount
        cached: true
        source: background
    }

    states: [
        State {
            name: "PLAIN"
            PropertyChanges { target: plainColor; visible: true }
        },
        State {
            name: "RADIAL"
            PropertyChanges { target: radial; visible: true }
        },
        State {
            name: "DIRECTORY"
            PropertyChanges { target: background; visible: !settings.blurBackground}
            PropertyChanges { target: fastBlur; visible: settings.blurBackground }
            PropertyChanges { target: updaterTimer; repeat: true }
        },
        State {
            name: "COVER"
            PropertyChanges { target: background; visible: !settings.blurBackground}
            PropertyChanges { target: fastBlur; visible: settings.blurBackground }
        }
    ]

    Component.onCompleted: update()
}
