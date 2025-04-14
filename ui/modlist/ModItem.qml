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

    property var instance
    property var prevObject: null
    property bool selected: false
    property var cDialog: null
    property var centerTo: null
    property var itemColor: "#77956C"
    anchors.fill: parent
    signal selectedMod(var mod)

    Component.onCompleted: {
        if (modItem.instance) {
            modItem.instance.uiComponent = modItem            
        } else {
            console.log("MODEL OBJECT IS NULL")
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
        opacity: determineDisabled() ? 1.0 : 0.5

        function determineBackgroundColor() {
            if (modArea.containsPress) {
                return Qt.darker(modItem.itemColor, 1.25)
            } else if (selected) {
                return Qt.darker(modItem.itemColor, 1.15)
            } else if (modArea.containsMouse) {
                return Qt.darker(modItem.itemColor, 1.10)
            } else {
                return Qt.darker(modItem.itemColor, 1.10)
            }
        }

        function determineDisabled() {
            if (modItem.instance) {
                return modItem.instance.enabled
            }
        }

        MouseArea {
            id: modArea
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            z: -1
            onClicked: function(mouse) {
                // Sanity checks
                console.log("Clicked on mod:", modItem.title)
                if (modItem.instance) {
                    console.log("Clicked on modItem:", modItem.instance.title)
                } else {
                    console.log("Clicked on modItem: NULL")
                }

                // Ctrl + left click adds to selection
                if (mouse.button === Qt.LeftButton && (mouse.modifiers & Qt.ControlModifier)) {
                    modController.addModToSelection(modItem.instance)
                    modItem.selected = true

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modController.selectedMods.length; i++) {
                        console.log(modController.selectedMods[i].title)
                    }
                }

                // Left click selects single mod
                else if (mouse.button === Qt.LeftButton) {
                    modController.clearSelection()
                    modController.setCurrentMod(modItem.instance)
                    modItem.selected = true

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modController.selectedMods.length; i++) {
                        console.log(modController.selectedMods[i].title)
                    }
                }

                // Right click only clears selection if the clicked mod isn't already selected
                else if (mouse.button === Qt.RightButton) {
                    const selected = modController.selectedMods
                    let alreadySelected = false
                    for (let i = 0; i < selected.length; i++) {
                        if (selected[i] === modItem.instance) {
                            alreadySelected = true
                            break
                        }
                    }

                    if (!alreadySelected) {
                        modController.clearSelection()
                        modController.setCurrentMod(modItem.instance)
                        modItem.selected = true
                    }

                    // set the selection so that the context menu gets updated array
                    modContextMenu.selection = modController.selectedMods
                    modContextMenu.popup()

                    // print selected mods
                    console.log("Selected mods:")
                    for (let i = 0; i < modController.selectedMods.length; i++) {
                        console.log(modController.selectedMods[i].title)
                    }
                }
            }

            hoverEnabled: true

            Menu {
                property var selection: modController.selectedMods
                id: modContextMenu
                // Disable/Enable mods
                MenuItem {
                    text: modContextMenu.selection.length > 1
                        ? (modItem.instance.enabled ? "Disable (" + modContextMenu.selection.length + ") mods"
                                                    : "Enable (" + modContextMenu.selection.length + ") mods")
                        : (modItem.instance.enabled ? "Disable mod" : "Enable mod")
                    onTriggered: {
                        console.log("Disabling mods")
                        modController.setSelectedModsEnabled(!(modItem.instance.enabled))
                        console.log("Disabled mods")
                    }
                }
                // Enable mods
                MenuItem {
                    text: modContextMenu.selection.length > 1 ? "Delete (" + modContextMenu.selection.length + ") mods" : "Delete mod"
                    onTriggered: {
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
                // Divider
                MenuSeparator {
                    visible: modContextMenu.selection.length > 1
                }
                // Select all mods
                MenuItem {
                    text: "Select all"
                    onTriggered: {
                        console.log("Selecting all mods")
                        modController.selectAll()
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
                    color: "#6B8760"
                    Layout.preferredWidth: 64
                    Layout.fillHeight: true
                    
                    Image {
                        id: modIcon

                        Component.onCompleted: {
                            if (modItem.instance) {
                                console.log("Checking icon for modItem:" + modItem.instance.title + " - " + modItem.instance.iconpaths.length + " icons" + " - " + modItem.instance.iconpaths[0])
                            } else {
                                console.log("MODEL OBJECT IS NULL")
                            }
                        }

                        property int radius: 5

                        Layout.preferredWidth: 44
                        Layout.preferredHeight: 31
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        source: (modItem.instance && modItem.instance.iconpaths.length > 0
                                ? "file:///" + modItem.instance.iconpaths[0]
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
                        id: modName
                        leftPadding: 10
                        text: modItem.title
                        font.pixelSize: 12
                        color: "#E8E8CF"
                    }

                    DropShadow {
                        anchors.fill: modName
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
                    checked: modItem.instance ? modItem.instance.enabled : false
                    Material.accent: "#376a3e"
                    enabled: true
                    onCheckedChanged: {
                        if (modItem.instance) {
                            console.log("Checkbox changed:", modItem.title, checked)
                            modController.clearSelection()
                            modController.setCurrentMod(modItem.instance)
                            modItem.selected = true
                            modController.setSelectedModsEnabled(checked)
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
        
        // allows selected to be dynamic; true only if the modItem matches currentMod
        // (makes select and deselect work)
        Connections {
            target: modController
            function onCurrentModChanged() {
                modItem.selected = (modController.currentMod === modItem.instance)
            }

            function onSelectedModsListUpdated() {
                modItem.selected = modController.selectedMods.includes(modItem.instance);
            }

        }


    }
}
