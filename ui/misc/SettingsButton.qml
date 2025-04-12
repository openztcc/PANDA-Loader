import QtQuick 2.15
import QtQuick.Controls.Material

Button {
    id: control
    width: parent ? parent.width : 200
    height: 50
    flat: true
    padding: 0
    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0
    anchors.topMargin: 0
    anchors.bottomMargin: 0

    background: Rectangle {
        id: bg
        property bool hovered: false
        radius: 0
        color: control.hovered ? "#66bb6a" : "#4caf50"
    }

    contentItem: Item {
        anchors.fill: parent
        Text {
            anchors.centerIn: parent
            text: control.text
            color: "white"
            font.bold: true
            font.pixelSize: 14
        }
    }
}
