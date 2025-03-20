import QtQuick

Item {
    id: filterTag
    width: implicitWidth // width based on content size
    height: 20
    required property string filter

    Rectangle {
        height: parent.height
        width: implicitWidth
        color: "#424940"
        Text {
            text: filterTag.filter
        }

    }
}
