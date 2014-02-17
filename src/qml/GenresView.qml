import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import MediaCenter 1.0

ScrollView {
    id: genresView

    function load() {
        filtersModel.closeIndex()
        filtersModel.update()
    }

    ListView {
        onActiveFocusChanged: {
            if (!activeFocus) {
                currentIndex = 0
                currentIndex = -1
            }
        }
        width: genresView.viewport.width
        model: GenreModel {
            id: filtersModel
            onGenreFiltersChanged: Backend.genreFilters = genreFilters
            Component.onCompleted: {
                genreFilters = Backend.genreFilters
                Backend.indexChanged.connect(load)
                load()
            }
        }
        cacheBuffer: count * 50
        delegate: CheckBox {
            property bool genreEnabled: roleEnabled
            onGenreEnabledChanged: checked = genreEnabled
            activeFocusOnPress: true
            onActiveFocusChanged: {
                if (activeFocus) {
                    ListView.view.currentIndex = index
                }
            }

            text: "(" + roleMediaCount + ") " + roleName
            onCheckedChanged: {
                if (checked !== genreEnabled) {
                    filtersModel.toggleGenre(index)
                }
            }
        }
    }
}
