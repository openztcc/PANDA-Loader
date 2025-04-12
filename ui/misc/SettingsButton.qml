import QtQuick 2.15
import QtQuick.Controls.Material
import PandaUI 1.0

Button {
    id: control
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

    property alias icon: icon.source

    background: Rectangle {
        id: bg
        property bool hovered: false
        anchors.fill: parent
        radius: 0
        color: control.hovered ? "#66bb6a" : "#4caf50"
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
            icon: control.icon
            color: "white"
        }

        Text {
            text: control.text
            color: "white"
            font.bold: true
            font.pixelSize: 12
        }
    }
}
