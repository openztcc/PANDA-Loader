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
        
        // Item to create a padding
        Item {
            anchors.fill: parent 
            anchors.margins: 12
            // mod title
            Text {
                id: modDetailsText
                text: infoPane.targetComponent ? infoPane.targetComponent.modTitle : "No mod selected"
                anchors.top: parent.top
                color: "#424940"
                font.pixelSize: 16
            }

            // description
            Text {
                id: modDetailsDesc
                text: infoPane.targetComponent ? infoPane.targetComponent.modDescription : "No description"
                anchors.top: modDetailsText.bottom
                color: "#424940"
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                width: parent.width
            }

            // authors
            InfoPaneItem {
                fieldName: "Authors"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.modAuthors : "No authors"
                iconImg: "qrc:/icons/author.svg"
                anchors.top: modDetailsDesc.bottom
            }
        }

    }

    // Connections {
    //     target: infoPane.targetComponent
    //     onSelectedMod: (mod) => {
    //         modDetailsText.text = mod.modTitle
    //         console.log("Selected mod: " + mod.modTitle)
    //     }
    // }
}
