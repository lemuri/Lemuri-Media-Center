import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Item {
    id: root
    function load() {
        filtersModel.closeIndex()
        filtersModel.update()
    }
    property int columValue: 60
    property int totalMediaCount: Backend.mediaABLFCount + Backend.mediaWoCopyrightCount + Backend.mediaClipsCount

    GridLayout {
        anchors.fill: parent
        anchors.margins: 4
        columns: 2
        rows: 2

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true
            title: "Gêneros habilitados"

            ColumnLayout {
                anchors.fill: parent

                GenresView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
