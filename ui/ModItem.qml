import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import PandaLdr 1.0

Item {
    id: modItem
    property var controller: null
    property var modelObject: null
    property var prevObject: null
    property bool isSelected: false
    anchors.fill: parent
    signal selectedMod(var mod)

    Component.onCompleted: {
        if (modItem.modelObject) {
            modItem.modelObject.qmlItem = modItem            
        }
    }

    Pane {
        id: modPane
        anchors.fill: parent
        Material.background: determineBackgroundColor()
        leftPadding: 10
        rightPadding: 10
        // topPadding: -5
        anchors.bottomMargin: 1

        function determineBackgroundColor() {
            if (modArea.containsPress) {
                return Qt.darker("#f7fbf2", 1.2)
            } else if (isSelected) {
                return Qt.darker("#f7fbf2", 1.1)
            } else if (modArea.containsMouse) {
                return Qt.lighter("#f7fbf2", 1.05)
            } else {
                return "#f7fbf2"
            }
        }
        
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
                        onClicked: function(mouse) {
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

                    // set new current mod 
                    modItem.controller.setCurrentMod(modItem.modelObject);
                    modItem.isSelected = true
                }

                // right click context menu
                if (mouse.button === Qt.RightButton) {
                    modContextMenu.popup()
                }
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

        // allows isSelected to be dynamic; true only if the modItem matches currentMod
        // (makes select and deselect work)
        Connections {
            target: modItem.controller
            function onCurrentModChanged() {
                modItem.isSelected = (modItem.controller.currentMod === modItem.modelObject)
            }
        }


    }
}
