import QtQuick 2.1

AnimatedDialog {
    id: root
    focus: true
    anchors.fill: parent

    property int itemHeight: 40
    property int itemWidth: settings.fontM * 1.5
    property string startsWithFilter: ""

    onShowChanged: {
        if (show) {
            hideTimer.restart()
        } else {
            hideTimer.stop()
        }
    }

    Frame {
        anchors.fill: filterColumn
        anchors.margins: -10
    }

    Component {
        id: letterDelegate
        Item {
            id: root
            width: itemWidth
            height: itemHeight

            property bool isSelected: root.ListView.isCurrentItem
            property string title: model.letter

            Rectangle {
                opacity: isSelected ? 0.8 : 0
                radius: 5
                anchors.fill: parent
                color: settings.selectionColor
            }

            Text {
                id: textItem
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: settings.fontM
                color: isSelected ? settings.fontShadowColor : settings.fontColorActive
                style: Text.Raised
                styleColor: isSelected ? settings.fontColorActive : settings.fontShadowColor
                text: title
            }
        }
    }

    Column {
        id: filterColumn
        anchors.centerIn: parent
        spacing: 4

        Text {
            id: textItem
            height: contentHeight
            width: contentWidth
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: settings.fontMM
            color: settings.fontColorActive
            style: Text.Raised
            styleColor: settings.fontShadowColor
            text: "Ir ao primeiro artista começando com a letra:"
        }

        ListView {
            id: listView
            height: itemHeight
            width: count * itemWidth + count * spacing
            spacing: 2
            model: alphabet
            orientation: ListView.Horizontal
            delegate: letterDelegate

            keyNavigationWraps: true
        }
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Left || event.key === Qt.Key_Down) {
            listView.decrementCurrentIndex()
            hideTimer.restart()
            event.accepted = true
        } else if (event.key === Qt.Key_Right || event.key === Qt.Key_Up) {
            listView.incrementCurrentIndex()
            hideTimer.restart()
            event.accepted = true
        }
    }

    Timer {
        id: hideTimer
        interval: 3000
        onTriggered: {
            startsWithFilter = ""
            root.show = false
        }
    }

    function select(event) {
        var currentItem = listView.currentItem
        if (currentItem) {
            startsWithFilter = currentItem.title
        }

        show = false
        event.accepted = true
    }

    ListModel {
        id: alphabet
        ListElement { letter: "A" }
        ListElement { letter: "B" }
        ListElement { letter: "C" }
        ListElement { letter: "E" }
        ListElement { letter: "F" }
        ListElement { letter: "G" }
        ListElement { letter: "H" }
        ListElement { letter: "I" }
        ListElement { letter: "J" }
        ListElement { letter: "K" }
        ListElement { letter: "L" }
        ListElement { letter: "M" }
        ListElement { letter: "N" }
        ListElement { letter: "O" }
        ListElement { letter: "P" }
        ListElement { letter: "Q" }
        ListElement { letter: "R" }
        ListElement { letter: "S" }
        ListElement { letter: "T" }
        ListElement { letter: "U" }
        ListElement { letter: "V" }
        ListElement { letter: "W" }
        ListElement { letter: "X" }
        ListElement { letter: "Y" }
        ListElement { letter: "Z" }
    }

    Keys.onSpacePressed: select(event)
    Keys.onEnterPressed: select(event)
    Keys.onReturnPressed: select(event)
}
