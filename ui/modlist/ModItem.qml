import QtQuick 2.15
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects
import PandaUI 1.0

pragma ComponentBehavior: Bound

Item {
    id: modItem

    property string title: "Unknown"

    property var mod
    property var prevObject: null
    property bool selected: false
    property var cDialog: null
    property var centerTo: null
    property var itemColor: "#77956C"
    property var disabledColor: "#5F7955"
    anchors.fill: parent
    signal selectedMod(var mod)

    Component.onCompleted: {
        if (modItem.mod) {
            modItem.mod.uiComponent = modItem            
        } else {
            console.log("MODEL OBJECT IS NULL")
        }
    }

    Pane {
        id: modPane
        anchors.fill: parent
        leftPadding: 10
        rightPadding: 10
        // topPadding: -5

        Material.background: modItem.mod.selected 
            ? Qt.darker(modItem.itemColor, 1.15)
            : modArea.containsMouse 
                ? Qt.darker(modItem.itemColor, 1.10) 
                : modArea.containsPress 
                    ? Qt.darker(modItem.itemColor, 1.25) 
                    : Qt.darker(modItem.itemColor, 1.05)

        function updateBackground() {
            return modItem.mod.enabled 
            ? modPane.determineBackgroundColor(modItem.itemColor) 
            : modPane.determineBackgroundColor(modItem.disabledColor)
        }

        function determineBackgroundColor(_color) {
            console.log("Determining background color for modItem:", modItem.mod.title)
            console.log("Is modItem selected:", modItem.mod.selected)
            if (modArea.containsPress && !determineDisabled()) {
                return Qt.darker(_color, 1.25)
            } else if (modItem.mod.selected) {
                return Qt.darker(_color, 1.15)
            } else if (modArea.containsMouse) {
                return Qt.darker(_color, 1.10)
            } else {
                return Qt.darker(_color, 1.05)
            }
        }

        function determineDisabled() {
            if (modItem.mod) {
                return !modItem.mod.enabled
            }
        }


        Rectangle {
            height: 1
            width: parent.width
            color: Qt.darker(modItem.itemColor, 1.2)
        }

        MouseArea {
            id: modArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onContainsMouseChanged: {
                modPane.Material.background = modPane.updateBackground()
            }

            onClicked: function(mouse) {
                // Sanity checks
                console.log("Clicked on mod:", modItem.mod.title)
                if (modItem.mod) {
                    console.log("Clicked on modItem:", modItem.mod.title)
                } else {
                    console.log("Clicked on modItem: NULL")
                }

                // Ctrl + left click adds to selection
                if (mouse.button === Qt.LeftButton && (mouse.modifiers & Qt.ControlModifier)) {
                    modController.setModSelected(modItem.mod.index, true)
                }

                // Left click selects single mod
                else if (mouse.button === Qt.LeftButton) {
                    modController.clearSelection()
                    modController.setCurrentMod(modItem.mod.index)
                    modController.setModSelected(modItem.mod.index, true)
                }

                // Right click only clears selection if the clicked mod isn't already selected
                else if (mouse.button === Qt.RightButton) {
                    let alreadySelected = modController.isModSelected(modItem.mod.index)

                    if (!alreadySelected) {
                        modController.clearSelection()
                        modController.setCurrentMod(modItem.mod.index)
                        modController.setModSelected(modItem.mod.index, true)
                    }

                    // set the selection so that the context menu gets updated array
                    modContextMenu.popup()
                }
            }

            hoverEnabled: true

            Menu {
                property var selectionCount: modController.selectedModsCount()
                id: modContextMenu
                // Disable/Enable mods
                MenuItem {
                    text: selectionCount > 1
                        ? (modItem.mod.enabled ? "Disable (" + selectionCount + ") mods"
                                                    : "Enable (" + selectionCount + ") mods")
                        : (modItem.mod.enabled ? "Disable mod" : "Enable mod")
                    onTriggered: {
                        console.log("Disabling mods")
                        modController.setSelectedModsEnabled(!(modItem.mod.enabled))
                        console.log("Disabled mods")
                    }
                }
                // Enable mods
                MenuItem {
                    text: selectionCount > 1 ? "Delete (" + selectionCount + ") mods" : "Delete mod"
                    onTriggered: {
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
                // Divider
                MenuSeparator {
                    visible: selectionCount > 1
                }
                // Select all mods
                MenuItem {
                    text: "Select all"
                    onTriggered: {
                        console.log("Selecting all mods")
                        modController.selectAllMods()
                        console.log("Selected all mods")
                    }
                }
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
                    rightMargin: 20
                }
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter

                Rectangle {
                    Layout.alignment: Qt.AlignLeft
                    color: modItem.mod.enabled ? modPane.determineBackgroundColor("#6B8760") : modPane.determineBackgroundColor(modItem.disabledColor)
                    Layout.preferredWidth: 64
                    Layout.fillHeight: true
                    
                    Image {
                        id: modIcon

                        Component.onCompleted: {
                            if (modItem.mod) {
                                console.log("Checking icon for modItem:" + modItem.mod.title + " - " + modItem.mod.iconpaths.size + " icons" + " - " + modItem.mod.iconpaths[0])
                            } else {
                                console.log("MODEL OBJECT IS NULL")
                            }
                        }

                        property int radius: 5

                        Layout.preferredWidth: 44
                        Layout.preferredHeight: 31
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        source: (modItem.mod && modItem.mod.iconpaths.length > 0
                                ? "file:///" + modItem.mod.iconpaths[0]
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
                }
                
                // mod name and category label
                RowLayout {
                    Layout.fillWidth: true
                    // name of mod
                    Text {
                        Layout.alignment: Qt.AlignTop
                        id: modName
                        leftPadding: 10
                        text: modItem.mod.title
                        font.pixelSize: 12
                        color: "#E8E8CF"
                    }

                    DropShadow {
                        Layout.alignment: Qt.AlignTop
                        source: modName
                        horizontalOffset: 2
                        verticalOffset: 2
                        radius: 4
                        samples: 16
                        color: "#2f000000" // semi-transparent black
                    }
                }
                
                Item {
                    Layout.fillWidth: true
                }
                
                // disable checkbox
                PCheckBox {
                    id: modCheck
                    z: 1
                    Layout.alignment: Qt.AlignRight
                    checked: modItem.mod ? modItem.mod.enabled : false
                    Material.accent: "#376a3e"
                    enabled: true
                    onCheckChanged: (checked) => {
                        if (modItem.mod) {
                            console.log("Checkbox changed:", modItem.mod.title, checked)
                            modController.clearSelection()
                            modController.setCurrentMod(modItem.mod.index)
                            modController.setModSelected(modItem.mod.index, checked)
                        }
                    }
                    
                    // Prevent click propagation to parent MouseArea
                    MouseArea {
                        anchors.fill: parent
                        onClicked: function(mouse) {     
                            mouse.accepted = true
                            modCheck.onCheckChanged(!modCheck.checked)
                        }
                    }
                }
            }
        }
        
        // allows selected to be dynamic; true only if the modItem matches currentMod
        // (makes select and deselect work)
        Connections {
            target: mod
            onIsSelectedChanged: {
                console.log("Mod selected state changed:", modItem.mod.title, modItem.mod.isSelected)
                modPane.Material.background = modPane.updateBackground()
            }

            onModSelected: {
                console.log("Mod selected signal received:", modItem.mod.title, modItem.mod.isSelected)
                modPane.Material.background = modPane.updateBackground()
            }

            onModDeselected: {
                console.log("Mod deselected signal received:", modItem.mod.title, modItem.mod.isSelected)
                modPane.Material.background = modPane.updateBackground()
            }
        }


    }
}
