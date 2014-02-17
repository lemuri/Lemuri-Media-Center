import QtQuick 2.1
import QtQuick.Controls 1.0

Button {
    property int key
    signal saveKey(int key)

    onActiveFocusChanged: checked = false
    activeFocusOnPress: true
    checkable: true
    text: checked ? "Pressione uma tecla..." : KeyManager.keyString(key)
    Keys.onPressed: {
        if (checked) {
            saveKey(event.key)
            checked = false
        }
        event.accepted = true
    }
}
