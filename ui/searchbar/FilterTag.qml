import QtQuick

Item {
    id: filterTag
    width: tagText.implicitWidth + 2 // width based on content size
    height: 20

    required property string filter

    Rectangle {
        id: tagRect
        height: parent.height
        width: tagText.width + 10
        color: Qt.darker("#627D58", 1.3)
        topLeftRadius: 3
        bottomLeftRadius: 3
        Text {
            id: tagText
            anchors.centerIn: parent
            text: filterTag.filter
            color: "#ffffff"
        }

    }
}
