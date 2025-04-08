import QtQuick 2.15
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects
import PandaLdr 1.0

pragma ComponentBehavior: Bound

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
        modItem.forceActiveFocus()
    }

    Keys.onPressed: (event) => {
        // Escape key to deselect mod
        if (event.key === Qt.Key_Escape) {
            var modTitle = modItem.modelObject.modTitle
            modItem.isSelected = false
            modItem.controller.clearSelection()
            console.log("Deselected mod:", modTitle)
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

                Image {
                    id: modIcon

                    property int radius: 5

                    Layout.preferredWidth: 44
                    Layout.preferredHeight: 31

                    source: (modItem.modelObject && modItem.modelObject.modIconPaths.length > 0
                             ? modItem.modelObject.modIconPaths[0]
                             : "")
                    fillMode: Image.PreserveAspectCrop
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        id: iconMask
                        maskSource: Rectangle {
                            id: rectMask
                            width: modIcon.width
                            height: modIcon.height
                            radius: modIcon.radius
                        }
                    }
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
                console.log("Clicked. Button:", mouse.button, "Modifiers:", mouse.modifiers)

                // Ctrl + left click adds to selection
                if (mouse.button === Qt.LeftButton && (mouse.modifiers & Qt.ControlModifier)) {
                    modItem.controller.addModToSelection(modItem.modelObject)
                    modItem.isSelected = true

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modItem.controller.selectedMods.length; i++) {
                        console.log(modItem.controller.selectedMods[i].modTitle)
                    }
                }

                // Left click selects single mod
                else if (mouse.button === Qt.LeftButton) {
                    modItem.controller.clearSelection()
                    modItem.controller.setCurrentMod(modItem.modelObject)
                    modItem.isSelected = true

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modItem.controller.selectedMods.length; i++) {
                        console.log(modItem.controller.selectedMods[i].modTitle)
                    }
                }

                // Right click only clears selection if the clicked mod isn't already selected
                else if (mouse.button === Qt.RightButton) {
                    const selected = modItem.controller.selectedMods
                    let alreadySelected = false
                    for (let i = 0; i < selected.length; i++) {
                        if (selected[i] === modItem.modelObject) {
                            alreadySelected = true
                            break
                        }
                    }

                    if (!alreadySelected) {
                        modItem.controller.clearSelection()
                        modItem.controller.setCurrentMod(modItem.modelObject)
                        modItem.isSelected = true
                    }

                    // set the selection so that the context menu gets updated array
                    modContextMenu.selection = modItem.controller.selectedMods
                    modContextMenu.popup()

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modItem.controller.selectedMods.length; i++) {
                        console.log(modItem.controller.selectedMods[i].modTitle)
                    }
                }
            }


            hoverEnabled: true

            Menu {
                property var selection: modItem.controller.selectedMods
                id: modContextMenu
                MenuItem {
                    id: modEdit
                    text: "Edit"
                    onTriggered: {
                        console.log("Option 1 triggered for", modItem.modelObject.modTitle)
                        // if more than 1 mod selected, disable option
                        if (modContextMenu.selection.length > 1) {
                            modEdit.enabled = false
                        } else {
                            modEdit.enabled = true
                        }
                    }
                }
                MenuItem {
                    text: modContextMenu.selection.length > 1 ? "Disable (" + modContextMenu.selection.length + ") mods" : "Disable mod"
                    onTriggered: {
                        console.log("Option 2 triggered for", modItem.modelObject.modTitle)
                    }
                }
                MenuItem {
                    
                    text: modContextMenu.selection.length > 1 ? "Delete (" + modContextMenu.selection.length + ") mods" : "Delete mod"
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

            function onSelectedModsListUpdated() {
                modItem.isSelected = modItem.controller.selectedMods.includes(modItem.modelObject);
            }

        }


    }
}
