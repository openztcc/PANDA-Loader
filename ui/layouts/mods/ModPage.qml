import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaLdr 1.0

Rectangle {
    anchors.fill: parent
    ColumnLayout {
        Layout.fillHeight: true // remaining height
        Layout.fillWidth: true
        anchors.fill: parent

        // launch area
        Rectangle {
            id: launchArea
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            Layout.alignment: Qt.AlignTop

            Image {
                id: bgImage
                source: "qrc:/images/launch_bg.jpg"
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop

                MultiEffect {
                    source: bgImage
                    anchors.fill: bgImage
                    saturation: 0.0
                }
            }

            RowLayout {
                width: parent.width
                height: 70
                spacing: 8
                anchors.bottom: parent.bottom


                Item { Layout.fillWidth: true }  // Spacer to push buttons to the right

                Button {
                    Material.background: "#376a3e"
                    Material.foreground: "#d4e8d1"
                    text: "Launch"

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: PState.launchZT()
                    }


                }

                Button {
                    text: "Install OpenZT"
                    Material.foreground: "#376a3e"
                    Material.background: "#9cd49f"

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: console.log("Install OpenZT clicked")
                    }
                }

                Item {
                    Layout.preferredWidth: 12
                }
            }

        }

        RowLayout {
            // mods list
            Layout.topMargin: 2
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                SearchBar {
                    id: searchBar
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    property string orderBy: ""
                    property string searchTerm: ""

                    onFilterBy: (filter) => {
                        console.log("Filter by:", filter)
                        searchBar.orderBy = filter
                    }

                    onSearchTextChanged: (text) => {
                        console.log("Search text changed:", text)
                        searchBar.searchTerm = text
                        modController.updateModList(searchBar.orderBy, searchBar.searchTerm)
                    }
                }

                Rectangle {
                    id: listHead
                    Layout.preferredHeight: 50
                    Layout.fillWidth: true
                    Layout.bottomMargin: -5
                    color: "#e3e8dd"

                    RowLayout {
                        anchors.fill: parent

                        Text {
                            text: "Mod Name"
                            Layout.alignment: Qt.AlignLeft
                            anchors.leftMargin: 10
                        }

                        Text {
                            text: "Enabled"
                            Layout.alignment: Qt.AlignRight
                        }

                        CheckBox {
                            id: headCheck
                            Layout.alignment: Qt.AlignRight
                            anchors.rightMargin: 10
                            Material.accent: "#376a3e"
                            checked: true
                        }
                    }
                }
                // mod list view
                ListView {                        
                    id: modsList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    boundsBehavior: Flickable.StopAtBounds
                    model: modModel
                    clip: true
                    focus: true

                    Component.onCompleted: {
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
                                selectedMods[i].isSelected = false
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

                        required property int index
                        required property var model
                        required property string title
                        required property var instance

                        Component.onCompleted: {
                            console.log("Delegate created for item at index:", modPane.index)
                            console.log("title:", modPane.title)
                            console.log("instance:", modPane.instance)
                            console.log("Available roles:", Object.keys(modPane.model).join(", "))
                        }

                        // bottom border
                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 1
                            color: "#c1c9be"
                        }

                        // mod list item
                        ModItem {
                            id: modItems
                            title: modPane.title
                            instance: modPane.instance
                            cDialog: confirmDialog
                            centerTo: mainContent
                        }
                    }

                    ScrollBar.vertical: ScrollBar {
                        id: vScroll
                        policy: ScrollBar.AsNeeded
                        visible: modsList.moving || modsList.dragging
                        width: 8
                        // background: Rectangle {
                        //     color: "#289662"
                        //     radius: 4
                        // }

                        contentItem: Rectangle {
                            color: "#486d48"
                            radius: 4
                            opacity: vScroll.hovered ? 1.0 : 0.7
                        }
                    }
                }
            }


            // mod details
            InfoPane {
                id: infoPane
                Connections {
                    target: modController
                    function onCurrentModChanged() {
                        infoPane.targetComponent = modController.currentMod;
                    }
                }
            }

        }

        // status bar
        Rectangle {
            Layout.bottomMargin: -5
            Layout.rightMargin: -5
            Layout.leftMargin: -5
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            Material.background: "#f7fbf2"

            Text {
                id: statusLabel
                text: "Status: " + statusLabel.model
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                color: "#424940"
                font.pixelSize: 10

                property string model: "No status to show"
            }
        }

    }

}
