import QtQuick 2.15
import QtQuick.Controls.Material
import PandaUI 1.0

Button {
    id: control
    property var color: "#2c6431"
    property var fg: "#E8E8CF"
    width: parent ? parent.width : 200
    height: 35
    flat: true
    padding: 0
    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0
    anchors.topMargin: 0
    anchors.bottomMargin: 0

    required property string source

    background: Rectangle {
        id: bg
        property bool hovered: false
        anchors.fill: parent
        radius: 0
        color: control.hovered ? Qt.darker(control.color, 1.2) : control.color

        Rectangle {
            height: 1
            anchors.bottom: parent.bottom
            width: parent.width
            color: Qt.darker(control.color, 1.2)
        }
    }

    contentItem: Item {
        anchors.fill: parent

        SvgIcon {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: 20
            height: 20
            icon: control.source
            color: control.fg
        }

        Text {
            text: control.text
            leftPadding: 40
            anchors.verticalCenter: parent.verticalCenter
            color: control.fg
            // font.bold: true
            font.pixelSize: 12
        }
    }
}
