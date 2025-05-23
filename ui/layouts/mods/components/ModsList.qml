import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import PandaUI 1.0

pragma ComponentBehavior: Bound

ColumnLayout {
    id: modList
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 0

    property var textColor: "#E8E8CF"
    property var itemColor: "#77956C"

    SearchBar {
        id: searchBar
        Layout.fillWidth: true
        Layout.preferredHeight: 35
        Layout.alignment: Qt.AlignTop
        property string orderBy: ""
        property string searchTerm: ""

        Rectangle {
            color: Qt.darker(modList.itemColor, 2.0)
            width: parent.width
            height: 1
            anchors.top: parent.top
        }

        // onFilterBy: (filter, search) => {
        //     console.log("Filter by:", filter)
        //     searchBar.orderBy = filter
        //     searchBar.searchTerm = search
        // }

        // onSearchTextChanged: (text) => {
        //     console.log("Search text changed:", text)
        //     searchBar.searchTerm = text
        //     modController.updateModList(searchBar.orderBy, searchBar.searchTerm)
        // }
    }

    Rectangle {
        id: listHead
        Layout.preferredHeight: 40
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#627D58" }
            GradientStop { position: 1.0; color: "#44603A" }
        }

        Rectangle {
            color: Qt.darker("#627D58", 1.2)
            anchors.top: parent.top
            width: parent.width
            height: 1
        }

        RowLayout {
            anchors.fill: parent

            Text {
                text: "Mod Name"
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 79
                color: modList.textColor
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                text: "Enabled"
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10
                color: modList.textColor
            }

            PCheckBox {
                id: headCheck
                Layout.alignment: Qt.AlignRight
                Material.accent: "#376a3e"
                checked: true
                Layout.rightMargin: 20
            }
        }
    }

    Item {
        //
        // Layout.fillWidth: true
        // Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.fillHeight: true

        Rectangle {
            anchors.fill: parent
            color: Qt.darker(modList.itemColor, 1.2)
        }

        // mod list view
        ListView {
            id: modsList
            anchors.fill: parent
            boundsBehavior: Flickable.StopAtBounds
            model: modModel
            clip: true
            focus: true
            cacheBuffer: 0

            Connections {
                target: modController
                onModelUpdated: {
                    console.log("Model updated")
                    modsList.model = null
                    modsList.model = modModel
                }
            }

            Component.onCompleted: {
                // load mods into the model
                modController.loadMods()
                
                modsList.forceActiveFocus(Qt.MouseFocusReason)
                console.log("ListView model:", modsList.model)
                console.log("Model type:", typeof modsList.model)
                console.log("Model row count:", modsList.model ? modsList.model.rowCount() : "N/A")
                console.log("Context property modModel:", modModel)
                console.log("Context property modController:", modController)
            }

            Keys.onPressed: (event) => {
                // Escape key to deselect mod
                if (event.key === Qt.Key_Escape) {
                    var selectedMods = modController.selectedMods

                    for (var i = 0; i < selectedMods.length; i++) {
                        selectedMods[i].setModSelected(i, false)
                    }
                    modController.clearSelection()
                    console.log("Deselected mods")
                }

                // Del key to delete mod
                else if (event.key === Qt.Key_Delete) {
                    var selectedCount = modController.selectedMods.length
                    // Ask for confirmation before deleting
                    confirmDialog.action = function() {
                        modController.controller.deleteSelected()
                        confirmDialog.close()
                    }
                    confirmDialog.title = "Delete " + (selectedCount > 1 ? selectedCount + " mods" : "mod")
                    confirmDialog.message = "Are you sure you want to delete " + (selectedCount > 1 ? selectedCount + " mods" : "this mod") + "?"
                    confirmDialog.centerTo = modsList.modPane
                    confirmDialog.open()
                }
            }


            delegate: Rectangle { // Mod list container
                id: modPane
                width: ListView.view.width
                height: 50

                // required property int index
                required property var model
                // required property string title
                // required property var instance

                Component.onCompleted: {
                    console.log("Delegate created for item at index:", model.index)
                    console.log("title:", model.title)
                    console.log("Description:", model.description)
                    console.log("instance:", model.instance)
                    console.log("Available roles:", Object.keys(model).join(", "))
                }

                // bottom border
                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 1
                    color: Qt.darker(modList.itemColor, 1.2)
                }

                // mod list item
                ModItem {
                    id: modItems
                    mod: model.instance
                    idx: model.index
                    cDialog: confirmDialog
                    centerTo: modPage
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: vScroll
                visible: true
                width: 10
                policy: ScrollBar.AlwaysOn
                // background: Rectangle {
                //     color: "#289662"
                //     radius: 4
                // }
                background: {
                    color: Qt.darker(modList.itemColor, 1.1)
                    width: 12
                }

                contentItem: Rectangle {
                    color: Qt.darker(modList.itemColor, 1.5)
                    radius: 4
                    opacity: vScroll.hovered ? 1.0 : 0.7
                }
            }

        }

    }

}
