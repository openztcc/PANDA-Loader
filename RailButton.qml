import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root
    width: parent ? parent.width : 80
    height: 56
    required property string text
    required property string icon
    required property string fg
    required property string bg

    ToolButton {
        anchors.fill: parent
        checkable: true
        text: root.text
        icon.source: root.icon
        font.pixelSize: 12
        Material.foreground: root.fg
        Material.background: root.bg

    }
}
