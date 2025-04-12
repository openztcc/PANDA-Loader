import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    property string text: "No Text"
    width: parent.width
    height: 50

    Button {
        text: "" + parent.text

        background: Rectangle {
            id: bg
            property bool hovered: false
            radius: 0
            color: hovered ? "#66bb6a" : "#4caf50"
        }

        padding: 0
        flat: true


        MouseArea {
           anchors.fill: parent
           hoverEnabled: true
           onEntered: bg.hovered = true
           onExited: bg.hovered = false
           acceptedButtons: Qt.NoButton
       }
    }
}
