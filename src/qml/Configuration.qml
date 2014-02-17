import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

AnimatedDialog {
    id: root

    function close() {
        KeyManager.filterKeys = true
        show = false
    }

    Rectangle {
        id: backgroundItem
        anchors.centerIn: parent
        width: parent.width - 50
        height: parent.height - 50
        color: sysPalette.window
    }

    Component {
        id: general
        ConfigGeneral {}
    }

    Component {
        id: configUi
        ConfigUI {}
    }

    Component {
        id: fileBrowser
        Loader {
            id: loader
            source: "FileBrowser.qml"

            Text {
                anchors.centerIn: parent
                visible: loader.status === Loader.Error
                text: "Falha ao carregar o navegador de arquivos."
            }
        }
    }

    Component {
        id: soundControl
        Loader {
            id: loader
            source: "SoundControl.qml"

            Text {
                anchors.centerIn: parent
                visible: loader.status === Loader.Error
                text: "Falha ao carregar o controle de volume."
            }
        }
    }

    GridLayout {
        anchors.fill: backgroundItem
        anchors.margins: 2
        columns: 2

        TabView {
            id: tabs
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Component.onCompleted: {
                addTab("Geral", general)
                addTab("Interface", configUi)
                addTab("Controle de Volume", soundControl)
            }
        }

        Button {
            property bool confirm: false
            onActiveFocusChanged: confirm = false
            activeFocusOnPress: true
            text: confirm ? "Pressione para Confirmar" :
                            KeyManager.filterKeys
                            ? "* Habilitar teclado real" : "* Desabilitar teclado real"
            onClicked: {
                if (confirm) {
                    KeyManager.filterKeys = !KeyManager.filterKeys
                    confirm = false
                } else {
                    confirm = true
                }
            }
        }

        Button {
            Layout.alignment: Qt.AlignRight
            text: "Fechar"
            onClicked: close()
        }
    }

    Keys.onEscapePressed: close()
}
