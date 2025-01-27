import QtQuick
import QtQuick.Effects


Item {
    id: root
    property string color: "#e5e9e1"
    property string fg: "#3a4b3a"
    property string icon: "No icon"
    property string text: ""
    implicitWidth: 56
    implicitHeight: parent.height
    signal clicked()

    Rectangle {
        width: parent.width
        height: parent.height
        radius: 0
        color: if (hoverArea.containsPress) {
            return Qt.lighter(root.color)
        } else if (hoverArea.containsMouse) {
            return Qt.darker(root.color)
        } else {
            return root.color
        }
        Behavior on color { ColorAnimation { duration: 70 } }


        Image {
            id: iconImage
            source: root.icon
            anchors.centerIn: parent
        }

        MultiEffect {
            source: iconImage
            anchors.fill: root
            colorization: 1.0
            colorizationColor: root.fg
        }

        Text {
            text: root.text
            font.pixelSize: 10
            color: "#3a4b3a"
            horizontalAlignment: Text.AlignHCenter
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
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
