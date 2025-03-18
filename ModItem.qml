import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: modItem
    property var controller: null
    property var model: null
    anchors.fill: parent
    
    Pane {
        anchors.fill: parent
        Material.background: modArea.containsPress ? Qt.lighter("#f7fbf2", 0.8) : 
                             modArea.containsMouse ? Qt.darker("#f7fbf2", 0.01) : "#f7fbf2"
        padding: 12
        anchors.bottomMargin: 1
        signal: clicked()
        
        contentItem: RowLayout {
            id: modMeta
            spacing: 12
            
            Rectangle {
                id: modImg
                width: 44
                height: 30
                color: "#BCD0C3"
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 3
                // category
                Label {
                    text: modItem.model && modItem.model.modCategory ? modItem.model.modCategory : "Uncategorized"
                    font.pixelSize: 10
                }
                // name of mod
                Label {
                    text: modItem.model && modItem.model.modTitle ? modItem.model.modTitle : "No title"
                    font.pixelSize: 12
                    color: "#424940"
                }
                // author(s)
                Label {
                    text: "by " + (modItem.model && modItem.model.modAuthor ? modItem.model.modAuthor : "Unknown")
                    font.pixelSize: 10
                }
            }
            
            Item {
                Layout.fillWidth: true
            }
            
            // disable checkbox
            CheckBox {
                id: modCheck
                z: 1
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: -15
                Layout.preferredWidth: 20
                checked: true
                Material.accent: "#376a3e"
                enabled: true
                onCheckedChanged: {
                    if (modItem.model) {
                        console.log("Checkbox changed:", modItem.model.modTitle, checked)
                    }
                }
                
                // Prevent click propagation to parent MouseArea
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        modCheck.toggle()
                        mouse.accepted = true
                    }
                }
            }
        }
        
        MouseArea {
            id: modArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                if (modItem.controller && modItem.model) {
                    modItem.controller.modClicked(modItem.model)
                    console.log("Mod clicked:", modItem.model.modTitle);
                    if (modDetailsText) {
                        modDetailsText.text = modItem.model.modDescription || "No description available";
                    }
                }
            }
            hoverEnabled: true
        }
    }
}