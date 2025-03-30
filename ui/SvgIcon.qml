import QtQuick
import QtQuick.Effects

Item {
    id: imgRoot

    required property string icon
    property int iconWidth: 24
    property int iconHeight: 24
    property int bgWidth: 24
    property int bgHeight: 24
    property string color: "#d4e8d1"

    width: bgWidth
    height: bgHeight

    Image {
        id: iconShape
        anchors.centerIn: parent
        source: imgRoot.icon
        width: imgRoot.iconWidth
        height: imgRoot.iconHeight
        fillMode: Image.PreserveAspectFit
        anchors.verticalCenter: imgRoot.verticalCenter

        // color: root.fg ? root.fg : "#d4e8d1"
    }

    MultiEffect {
        source: iconShape
        anchors.fill: iconShape
        colorization: 1.0
        colorizationColor: imgRoot.color
    }
}
