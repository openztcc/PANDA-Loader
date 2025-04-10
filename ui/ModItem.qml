import QtQuick 2.15
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects
import PandaLdr 1.0

pragma ComponentBehavior: Bound

Item {
    id: modItem

    property string modTitle: "Unknown"

    property var modelObject
    property var prevObject: null
    property bool isSelected: false
    property var cDialog: null
    property var centerTo: null
    anchors.fill: parent
    signal selectedMod(var mod)

    Component.onCompleted: {
        if (modItem.modelObject) {
            modItem.modelObject.qmlItem = modItem            
        } else {
            console.log("MODEL OBJECT IS NULL")
        }

        console.log("DELEGATE ACTIVE - modTitle:", typeof modTitle, modTitle)
    console.log("DELEGATE ACTIVE - modObject:", typeof modObject, modObject)
    }

    Pane {
        id: modPane
        anchors.fill: parent
        Material.background: determineBackgroundColor()
        leftPadding: 10
        rightPadding: 10
        // topPadding: -5
        anchors.bottomMargin: 1
        opacity: determineDisabled() ? 0.5 : 1.0

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

        function determineDisabled() {
            if (modItem.modelObject) {
                return modItem.modelObject.isDisabled
            } else {
                return false
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

                    Component.onCompleted: {
                        if (modItem.modelObject) {
                            console.log("Checking icon for modItem:" + modItem.modelObject.modTitle + " - " + modItem.modelObject.modIconPaths.length + " icons" + " - " + modItem.modelObject.modIconPaths[0])
                        } else {
                            console.log("MODEL OBJECT IS NULL")
                        }
                    }

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
                        text: modItem.modTitle
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
                            console.log("Checkbox changed:", modItem.modTitle, checked)
                            if (!checked) {
                                modController.disableMod(modItem.modelObject)
                            }
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

                console.log("Clicked on mod:", modItem.modTitle)
                if (modItem.modelObject) {
                    console.log("Clicked on modItem:", modItem.modelObject.modTitle)
                } else {
                    console.log("Clicked on modItem: NULL")
                }
                // print mod list
                console.log("Mod list:")
                var modList = modController.model.modsList()
                for (let i = 0; i < modList.length; i++) {
                    console.log(modList[i].modTitle)
                }

                // Ctrl + left click adds to selection
                if (mouse.button === Qt.LeftButton && (mouse.modifiers & Qt.ControlModifier)) {
                    modController.addModToSelection(modItem.modelObject)
                    modItem.isSelected = true

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modController.selectedMods.length; i++) {
                        console.log(modController.selectedMods[i].modTitle)
                    }
                }

                // Left click selects single mod
                else if (mouse.button === Qt.LeftButton) {
                    modController.clearSelection()
                    modController.setCurrentMod(modItem.modelObject)
                    modItem.isSelected = true

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modController.selectedMods.length; i++) {
                        console.log(modController.selectedMods[i].modTitle)
                    }
                }

                // Right click only clears selection if the clicked mod isn't already selected
                else if (mouse.button === Qt.RightButton) {
                    const selected = modController.selectedMods
                    let alreadySelected = false
                    for (let i = 0; i < selected.length; i++) {
                        if (selected[i] === modItem.modelObject) {
                            alreadySelected = true
                            break
                        }
                    }

                    if (!alreadySelected) {
                        modController.clearSelection()
                        modController.setCurrentMod(modItem.modelObject)
                        modItem.isSelected = true
                    }

                    // set the selection so that the context menu gets updated array
                    modContextMenu.selection = modController.selectedMods
                    modContextMenu.popup()

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modController.selectedMods.length; i++) {
                        console.log(modController.selectedMods[i].modTitle)
                    }
                }
            }

            hoverEnabled: true

            Menu {
                property var selection: modController.selectedMods
                id: modContextMenu
                MenuItem {
                    id: modEdit
                    text: "Edit"
                    onTriggered: {
                        console.log("Option 1 triggered for", modItem.modTitle)
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
                        console.log("Option 2 triggered for", modItem.modTitle)
                        var selectedCount = modContextMenu.selection.length
                        // Ask for confirmation before disabling
                        modItem.cDialog.action = function() {
                            console.log("Disabling mods")
                            modController.disableSelected()
                            console.log("Disabled mods")
                            modItem.cDialog.close()
                        }
                        modItem.cDialog.title = "Disable " + (selectedCount > 1 ? selectedCount + " mods" : "mod")
                        modItem.cDialog.message = "Are you sure you want to disable " + (selectedCount > 1 ? selectedCount + " mods" : "this mod") + "?"
                        modItem.cDialog.centerTo = modItem.centerTo
                        modItem.cDialog.open()
                    }
                }
                MenuItem {
                    text: modContextMenu.selection.length > 1 ? "Delete (" + modContextMenu.selection.length + ") mods" : "Delete mod"
                    onTriggered: {
                        console.log("Option 2 triggered for", modItem.modTitle)
                        var selectedCount = modContextMenu.selection.length
                        // Ask for confirmation before deleting
                        modItem.cDialog.action = function() {
                            modController.deleteSelected()
                            modItem.cDialog.close()
                        }
                        modItem.cDialog.title = "Delete " + (selectedCount > 1 ? selectedCount + " mods" : "mod")
                        modItem.cDialog.message = "Are you sure you want to delete " + (selectedCount > 1 ? selectedCount + " mods" : "this mod") + "?"
                        modItem.cDialog.centerTo = modItem.centerTo
                        modItem.cDialog.open()
                    }
                }
            }
        }

        // allows isSelected to be dynamic; true only if the modItem matches currentMod
        // (makes select and deselect work)
        Connections {
            target: modController
            function onCurrentModChanged() {
                modItem.isSelected = (modController.currentMod === modItem.modelObject)
            }

            function onSelectedModsListUpdated() {
                modItem.isSelected = modController.selectedMods.includes(modItem.modelObject);
            }

        }


    }
}
