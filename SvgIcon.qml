import QtQuick
import QtQuick.Effects

Item {
    required property string icon
    property int iconWidth: 24
    property int iconHeight: 24
    property int bgWidth: 24
    property int bgHeight: 24

    width: bgWidth
    height: bgHeight

    Image {
        id: imgRoot
        anchors.centerIn: parent
        source: parent.icon
        width: parent.iconWidth
        height: parent.iconHeight
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: parent.horizontalCenter

        // color: root.fg ? root.fg : "#d4e8d1"
    }

    MultiEffect {
        source: imgRoot
        anchors.fill: imgRoot
        colorization: 1.0
        colorizationColor: "#d4e8d1"
    }
}
