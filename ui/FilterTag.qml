import QtQuick
import QtQuick.Layouts

Item {
    id: filterTag
    width: tagText.implicitWidth + 2 // width based on content size
    height: 20

    required property string filter

    Rectangle {
        id: tagRect
        height: parent.height
        width: tagText.width + 10
        color: "#424940"
        radius: 5
        Text {
            id: tagText
            anchors.centerIn: parent
            text: filterTag.filter
            color: "#ffffff"
        }

    }
}
