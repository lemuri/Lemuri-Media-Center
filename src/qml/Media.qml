import QtQuick 2.1

FocusScope {
    id: root

    Background {
        anchors.fill: parent
        source: moviesSelector.absolutePath
    }

    Item {
        id: mainViewArea
        anchors.fill: parent

        MoviesSelector {
            id: moviesSelector
            focus: true
            anchors.fill: parent
            startsWithFilter: filterBar.startsWithFilter
            onChooseCD: {
//                musicView.album = title
//                musicView.absolutePath = absolutePath
//                root.state = "MEDIA"
                mediaPlayer.addMedia(absolutePath)
            }
            onShowFilterBar: root.state = "FILTER"
        }

        BarBackground {
            id: topBackground
            opacity: 0.5
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: topBar.height + barAnchors.bottomMargin
            barAnchors.bottomMargin: 15
        }

        Row {
            id: topBar
            height: titleLabel.contentHeight + spacing * 2
            anchors.left: topBackground.left
            anchors.top: topBackground.top
            anchors.right: topBackground.right
            anchors.leftMargin: spacing
            anchors.rightMargin: spacing
            spacing: 4

            Text {
                id: titleLabel
                font.pointSize: settings.fontS
                width: parent.width
                elide: Text.ElideRight
                color: settings.fontColorActive
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                style: Text.Raised
                styleColor: settings.fontShadowColor
                text: moviesSelector.title
            }
        }
    }

    MusicsView {
        id: musicView
        onShowChanged: {
            if (!show) {
                root.state = "MAIN"
            }
        }
    }

    ArtistSelector {
        id: filterBar
        onShowChanged: {
            if (!show) {
                root.state = "MAIN"
            }
        }
    }

    states: [
        State {
            name: "MAIN"
            PropertyChanges { target: moviesSelector; focus: true}
        },
        State {
            name: "MEDIA"
            PropertyChanges { target: musicView; show: true}
            PropertyChanges { target: musicView; focus: true}
        },
        State {
            name: "FILTER"
            PropertyChanges { target: filterBar; show: true}
            PropertyChanges { target: filterBar; focus: true}
        }
    ]
}
