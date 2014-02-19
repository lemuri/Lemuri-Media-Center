import QtQuick 2.1
import MediaCenter 1.0

Item {
    id: mainWindow
    width: 1024
    height: 768
    state: "RUNNING"

    SystemPalette { id: sysPalette }

    Item {
        id: settings
        // These are used to scale fonts according to screen size
        property real _scaler: 400 + mainWindow.width * mainWindow.height * 0.00015
        property int fontXS: _scaler * 0.024
        property int fontS: _scaler * 0.032
        property int fontM: _scaler * 0.040
        property int fontMM: _scaler * 0.046
        property int fontL: _scaler * 0.064
        // font colors
        property string fontColorActive: Backend.config("gui/fontColorActive", "#fff")
        onFontColorActiveChanged: Backend.setConfig("gui/fontColorActive", fontColorActive)
        property string fontShadowColor: Backend.config("gui/fontShadowColor", "#000")
        onFontShadowColorChanged: Backend.setConfig("gui/fontShadowColor", fontShadowColor)
        property string selectionColor: Backend.config("gui/selectionColor", "#fff")
        onSelectionColorChanged: Backend.setConfig("gui/selectionColor", selectionColor)
        // Visual settings
        property bool animations: Backend.config("gui/Animations", false)
        onAnimationsChanged: Backend.setConfig("gui/Animations", animations)
        property bool blurBackground: Backend.config("gui/blurBackground", false)
        onBlurBackgroundChanged: Backend.setConfig("gui/blurBackground", blurBackground)
        property string plainBackgroundColor: Backend.config("gui/PlainBackgroundColor", "grey")
        onPlainBackgroundColorChanged: Backend.setConfig("gui/PlainBackgroundColor", plainBackgroundColor)
        property string radialColor: Backend.config("gui/radialColor", "#FF000000")
        onRadialColorChanged: Backend.setConfig("gui/radialColor", radialColor)
        property string coverBackground: Backend.config("gui/CoverBackground", "COVER")
        onCoverBackgroundChanged: Backend.setConfig("gui/CoverBackground", coverBackground)
        property bool showFps: false
        onShowFpsChanged: {
            if (showFps) {
                fpsLoader.source = "FpsItem.qml"
            } else {
                fpsLoader.source = ""
            }
        }
    }

    Loader {
        id: fpsLoader
        z: 5
        anchors.right: parent.right
        anchors.top: parent.top
    }

    LemuriMediaCenter {
        id: mediaCenter
        anchors.fill: parent
        visible: false
    }

    Configuration {
        id: configUi
        onShowChanged: {
            if (!show) {
                mainWindow.state = "RUNNING"
            }
        }
    }

    Notifications {
        id: notifications
        anchors.fill: parent
        visible: mediaCenter.visible
    }

    MediaPlayer {
        id: mediaPlayer
        volume: Backend.volume
    }

    Volume {
        id: volume
    }

    Keys.onPressed: {
        if (!KeyManager.filterKeys) {
            return
        }

        var newState = "RUNNING"
        if (event.key === KeyManager.keyVolume) {
            if (!volume.show) {
                newState = "VOLUME"
            }
            event.accepted = true
        } else if (event.key === KeyManager.keyConfigure) {
            if (!configUi.show) {
                newState = "CONFIGURING"
            }
            event.accepted = true
        }

        if (event.accepted) {
            mainWindow.state = newState
        }
    }

    states: [
        State {
            name: "RUNNING"
            PropertyChanges { target: mediaCenter; visible: true}
            PropertyChanges { target: mediaCenter; focus: true}
        },
        State {
            name: "VOLUME"
            PropertyChanges { target: mediaCenter; visible: true}
            PropertyChanges { target: volume; show: true}
            PropertyChanges { target: volume; focus: true}
        },
        State {
            name: "CONFIGURING"
            PropertyChanges { target: mediaCenter; visible: true}
            PropertyChanges { target: configUi; show: true}
            PropertyChanges { target: configUi; focus: true}
            PropertyChanges { target: Backend; configuring: true}
        }
    ]
}
