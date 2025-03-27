import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import PandaLdr 1.0

Item {
    id: modItem
    property var controller: null
    property var modelObject: null
    property bool isClicked: false
    anchors.fill: parent
    signal selectedMod(var mod)

    Pane {
        anchors.fill: parent
        Material.background: modArea.containsPress ? Qt.darker("#f7fbf2", 1.2) :
                            isClicked ? Qt.darker("#f7fbf2", 1.1) :
                            modArea.containsMouse ? Qt.lighter("#f7fbf2", 1.05) : 
                            "#f7fbf2"
        leftPadding: 10
        rightPadding: 10
        // topPadding: -5
        anchors.bottomMargin: 1
        
        contentItem: Item { 
            anchors.fill: parent
            RowLayout {
                id: modMeta
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                    leftMargin: 10
                    rightMargin: 20
                }
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                
                // mod icon placeholder
                Rectangle {
                    id: modImg
                    width: 44
                    height: 30
                    color: "#BCD0C3"
                }
                
                // mod name and category label
                RowLayout {
                    Layout.fillWidth: true
                    // name of mod
                    Label {
                        id: modName
                        leftPadding: 10
                        text: modItem.modelObject && modItem.modelObject.modTitle ? modItem.modelObject.modTitle : "No title"
                        font.pixelSize: 12
                        color: "#424940"
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
                    Layout.preferredWidth: 20
                    checked: true
                    Material.accent: "#376a3e"
                    enabled: true
                    onCheckedChanged: {
                        if (modItem.modelObject) {
                            console.log("Checkbox changed:", modItem.modelObject.modTitle, checked)
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
        }
        
        MouseArea {
            id: modArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: function(mouse) {
                // left click to select mod
                if (modItem.controller && modItem.modelObject) {
                    // grab the mod object
                    var controller = modItem.controller
                    controller.setCurrentMod(modItem.modelObject)
                    // console.log("Mod clicked:", currentMod.modTitle)
                    // if (modDetailsText) {
                    //     modDetailsText.text = modItem.modelObject.modDescription || "No description available";
                    // }
                }

                // right click context menu
                if (mouse.button === Qt.RightButton) {
                    modContextMenu.popup()
                }

                // toggle click state
                isClicked = !isClicked
            }
            hoverEnabled: true

            Menu {
                id: modContextMenu
                MenuItem {
                    text: "Edit"
                    onTriggered: {
                        console.log("Option 1 triggered for", modItem.modelObject.modTitle)
                    }
                }
                MenuItem {
                    text: "Disable"
                    onTriggered: {
                        console.log("Option 2 triggered for", modItem.modelObject.modTitle)
                    }
                }
                MenuItem {
                    text: "Delete"
                    onTriggered: {
                        console.log("Option 2 triggered for", modItem.modelObject.modTitle)
                    }
                }
            }
        }
    }
}
