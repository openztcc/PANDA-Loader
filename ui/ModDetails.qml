import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: modDetails
    property string text: "No text"
    implicitWidth: 300
    Layout.fillHeight: true

    Rectangle {
        id: modDetailsPane
        color: "#f7fbf2"
        radius: 0
        anchors.fill: parent

        // mod title
        Text {
            id: modDetailsText
            text: ""
            anchors.centerIn: parent
            color: "#424940"
            font.pixelSize: 14
        }
    }
}
