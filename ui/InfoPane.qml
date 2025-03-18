import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: infoPane
    property string text: "No text"
    property var targetComponent: null
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
            text: targetComponent ? targetComponent.modTitle : "No mod selected"
            anchors.centerIn: parent
            color: "#424940"
            font.pixelSize: 14
        }
    }

    // Connections {
    //     target: targetComponent
    //     onSelectedMod: (mod) => {
    //         modDetailsText.text = mod.modTitle
    //         console.log("Selected mod: " + mod.modTitle)
    //     }
    // }
}
