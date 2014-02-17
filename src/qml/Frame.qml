import QtQuick 2.1

Rectangle {
    id: backgroundItem
    border.color: "#808080"
    border.width: 1
    opacity: 0.9
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#101010" }
        GradientStop { position: 0.3; color: "#404040" }
        GradientStop { position: 1.0; color: "#090909" }
    }
}
