import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4

        GroupBox {
            Layout.fillWidth: true
            title: "Aparência:"

            GridLayout {
                anchors.fill: parent
                columns: 3

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "Tamanho das capas:"
                }
                Slider {
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    maximumValue: 400
                    stepSize: 10
                    tickmarksEnabled: true
                    activeFocusOnPress: true
                    onValueChanged: Backend.coverSize = value
                    Keys.onPressed: {
                        if (event.key === Qt.Key_Plus) {
                            value += stepSize
                            event.accepted = true
                        } else if (event.key === Qt.Key_Minus) {
                            value -= stepSize
                            event.accepted = true
                        }
                    }
                    Component.onCompleted: {
                        value = Backend.coverSize
                        minimumValue = 100
                    }
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "Cor do texto*:"
                }
                TextField {
                    Layout.fillWidth: true
                    enabled: !KeyManager.filterKeys
                    activeFocusOnPress: true
                    text: settings.fontColorActive
                    onTextChanged: settings.fontColorActive = text
                }
                Rectangle {
                    Layout.fillHeight: true
                    width: 100
                    radius: 5
                    color: settings.fontColorActive
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "Cor da sombra do texto*:"
                }
                TextField {
                    Layout.fillWidth: true
                    enabled: !KeyManager.filterKeys
                    activeFocusOnPress: true
                    text: settings.fontShadowColor
                    onTextChanged: settings.fontShadowColor = text
                }
                Rectangle {
                    Layout.fillHeight: true
                    width: 100
                    radius: 5
                    color: settings.fontShadowColor
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "Cor da seleção*:"
                }
                TextField {
                    Layout.fillWidth: true
                    enabled: !KeyManager.filterKeys
                    activeFocusOnPress: true
                    text: settings.selectionColor
                    onTextChanged: settings.selectionColor = text
                }
                Rectangle {
                    Layout.fillHeight: true
                    width: 100
                    radius: 5
                    color: settings.selectionColor
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "Cor do fundo liso*:"
                }
                TextField {
                    Layout.fillWidth: true
                    enabled: !KeyManager.filterKeys
                    activeFocusOnPress: true
                    text: settings.plainBackgroundColor
                    onTextChanged: settings.plainBackgroundColor = text
                }
                Rectangle {
                    Layout.fillHeight: true
                    width: 100
                    radius: 5
                    color: settings.plainBackgroundColor
                }

                Label {
                    Layout.alignment: Qt.AlignRight
                    text: "Cor do fundo radial*:"
                }
                TextField {
                    Layout.fillWidth: true
                    enabled: !KeyManager.filterKeys
                    activeFocusOnPress: true
                    text: settings.radialColor
                    onTextChanged: settings.radialColor = text
                }
                Rectangle {
                    Layout.fillHeight: true
                    width: 100
                    radius: 5
                    color: settings.radialColor
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            title: "Desempenho:"

            GridLayout {
                anchors.fill: parent

                CheckBox {
                    activeFocusOnPress: true
                    text: "Mostrar quadros por segundo"
                    checked: settings.showFps
                    onCheckedChanged: settings.showFps = checked
                }
                CheckBox {
                    activeFocusOnPress: true
                    text: "Animações"
                    checked: settings.animations
                    onCheckedChanged: settings.animations = checked
                }
                CheckBox {
                    activeFocusOnPress: true
                    enabled: settings.coverBackground === "COVER" || settings.coverBackground === "DIRECTORY"
                    text: "Borrar plano de fundo"
                    checked: settings.blurBackground
                    onCheckedChanged: settings.blurBackground = checked
                }
                ComboBox {
                    Layout.fillWidth: true
                    property bool inited: false
                    activeFocusOnPress: true
                    model: ListModel {
                        id: cbItems
                        ListElement { text: "Fundo liso"; option: "PLAIN" }
                        ListElement { text: "Fundo radial"; option: "RADIAL" }
                        ListElement { text: "Capa do disco"; option: "COVER" }
                        ListElement { text: "Imagens do diretório PlanoDeFundo"; option: "DIRECTORY" }
                    }
                    onCurrentIndexChanged: {
                        if (inited) {
                            settings.coverBackground = cbItems.get(currentIndex).option
                        }
                    }
                    Component.onCompleted: {
                        var option = settings.coverBackground
                        if (option === "PLAIN") {
                            currentIndex = 0
                        } else if (option === "RADIAL") {
                            currentIndex = 1
                        } else if (option === "COVER") {
                            currentIndex = 2
                        } else if (option === "DIRECTORY") {
                            currentIndex = 3
                        }
                        inited = true
                    }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            enabled: !KeyManager.filterKeys
            title: "Mapeamento de teclas*:"

            ScrollView {
                anchors.fill: parent

                GridLayout {
                    columns: 2

                    Label { Layout.alignment: Qt.AlignRight; text: "Mover para cima:" }
                    ShortcutButton {
                        key: KeyManager.keyUp
                        onSaveKey: KeyManager.keyUp = key
                    }
                    Label { Layout.alignment: Qt.AlignRight; text: "Mover para baixo:" }
                    ShortcutButton {
                        key: KeyManager.keyDown
                        onSaveKey: KeyManager.keyDown = key
                    }
                    Label { Layout.alignment: Qt.AlignRight; text: "Mover para esquerda:" }
                    ShortcutButton {
                        key: KeyManager.keyLeft
                        onSaveKey: KeyManager.keyLeft = key
                    }
                    Label { Layout.alignment: Qt.AlignRight; text: "Mover para direita:" }
                    ShortcutButton {
                        key: KeyManager.keyRight
                        onSaveKey: KeyManager.keyRight = key
                    }

                    Label { Layout.alignment: Qt.AlignRight; text: "Escolher álbum:" }
                    ShortcutButton {
                        key: KeyManager.keyChooseAlbum
                        onSaveKey: KeyManager.keyChooseAlbum = key
                    }
                    Label { Layout.alignment: Qt.AlignRight; text: "Tocar mídia:" }
                    ShortcutButton {
                        key: KeyManager.keyPlayMedia
                        onSaveKey: KeyManager.keyPlayMedia = key
                    }
                    Label { Layout.alignment: Qt.AlignRight; text: "Cancelar mídia atual:" }
                    ShortcutButton {
                        key: KeyManager.keyCancelMedia
                        onSaveKey: KeyManager.keyCancelMedia = key
                    }

                    Label { Layout.alignment: Qt.AlignRight; text: "Mostrar volume:" }
                    ShortcutButton {
                        key: KeyManager.keyVolume
                        onSaveKey: KeyManager.keyVolume = key
                    }
                    Label { Layout.alignment: Qt.AlignRight; text: "Mostrar configurações:" }
                    ShortcutButton {
                        key: KeyManager.keyConfigure
                        onSaveKey: KeyManager.keyConfigure = key
                    }

                    Label { Layout.alignment: Qt.AlignRight; text: "Sair:" }
                    ShortcutButton {
                        key: KeyManager.keyLeave
                        onSaveKey: KeyManager.keyLeave = key
                    }
                }
            }
        }
    }
}
