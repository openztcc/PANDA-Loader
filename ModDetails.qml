import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: modDetails
    property string text: "No text"
    Layout.preferredWidth: 300
    Layout.fillHeight: true
    anchors.top: actionBar.bottom

    Rectangle {
        id: modDetailsPane
        color: "#f7fbf2"
        radius: 0

        Text {
            id: modDetailsText
            text: ""
            anchors.centerIn: parent
            color: "#424940"
            font.pixelSize: 14
        }
    }
}
