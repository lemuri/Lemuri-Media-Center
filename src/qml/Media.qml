import QtQuick 2.1

FocusScope {
    id: root

    Background {
        anchors.fill: parent
        source: albumSelector.absolutePath
    }

    Item {
        id: mainViewArea
        anchors.fill: parent

        AlbumSelector {
            id: albumSelector
            focus: true
            anchors.fill: parent
            startsWithFilter: filterBar.startsWithFilter
            onChooseCD: {
                musicView.album = album
                musicView.absolutePath = absolutePath
                root.state = "MEDIA"
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
            height: artistAlbumGrid.height + spacing * 2
            anchors.left: topBackground.left
            anchors.top: topBackground.top
            anchors.right: topBackground.right
            anchors.leftMargin: spacing
            anchors.rightMargin: spacing
            spacing: 4

            Grid {
                id: artistAlbumGrid
                anchors.verticalCenter: parent.verticalCenter
                columns: 2
                rows: 2
                spacing: 4
                width: parent.width - parent.spacing
                property int leftColSize: Math.max(artistL.contentWidth, albumL.contentWidth)

                Text {
                    id: artistL
                    width: artistAlbumGrid.leftColSize
                    horizontalAlignment: Text.AlignRight
                    visible: albumSelector.artist.length
                    font.pointSize: settings.fontS
                    color: settings.fontColorActive
                    style: Text.Raised
                    styleColor: settings.fontShadowColor
                    text: "Artista:"
                }
                Text {
                    font.pointSize: settings.fontS
                    width: parent.width - artistAlbumGrid.leftColSize
                    elide: Text.ElideRight
                    color: settings.fontColorActive
                    style: Text.Raised
                    styleColor: settings.fontShadowColor
                    text: albumSelector.artist
                }
                Text {
                    id: albumL
                    width: artistAlbumGrid.leftColSize
                    horizontalAlignment: Text.AlignRight
                    visible: albumSelector.album.length
                    font.pointSize: settings.fontS
                    color: settings.fontColorActive
                    style: Text.Raised
                    styleColor: settings.fontShadowColor
                    text: "Álbum:"
                }
                Text {
                    font.pointSize: settings.fontS
                    width: parent.width - artistAlbumGrid.leftColSize
                    elide: Text.ElideRight
                    color: settings.fontColorActive
                    style: Text.Raised
                    styleColor: settings.fontShadowColor
                    text: albumSelector.album
                }
            }
        }

        PlayingBar {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
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
            PropertyChanges { target: albumSelector; focus: true}
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
