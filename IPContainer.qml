import QtQuick

Item {
    id: root
    property string text: "No text"
    width: 80
    height: 40

    Rectangle {
        width: 80
        height: 40
        color: "#f7fbf2"

        Text {
            text: root.text  // Use the property defined in Item
            anchors.centerIn: parent
            font.pixelSize: 14
            color: "#000"
        }
    }
}
