import QtQuick
import QtQuick.Effects

Item {
    required property string icon

    Image {
        id: root
        anchors.centerIn: parent
        source: root.icon
        width: 24
        height: 24
        fillMode: Image.PreserveAspectFit
        // color: root.fg ? root.fg : "#d4e8d1"
    }

    MultiEffect {
        source: root
        anchors.fill: root
        colorization: 1.0
        colorizationColor: "#d4e8d1"
    }
}
