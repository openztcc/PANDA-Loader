import QtQuick
import QtQuick.Effects


Item {
    id: root
    property string color: "#e5e9e1"
    property string fg: "#3a4b3a"
    property string icon: "No icon"
    property string text: ""
    implicitWidth: 50
    implicitHeight: parent.height
    signal clicked()

    Rectangle {
        width: parent.width
        height: parent.height
        radius: 0
        color: if (hoverArea.containsPress) {
            return Qt.lighter(root.color, 0.8)
        } else if (hoverArea.containsMouse) {
            return Qt.darker(root.color, 0.01)
        } else {
            return root.color
        }
        Behavior on color { ColorAnimation { duration: 70 } }


        Image {
            width: 20
            height: 20
            id: iconImage
            source: root.icon
            anchors.centerIn: parent
        }

        MultiEffect {
            source: iconImage
            anchors.fill: iconImage
            colorization: 1.0
            colorizationColor: "#3a4b3a"
        }
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        onClicked: root.clicked()
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
    }


}
