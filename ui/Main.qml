import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import PandaLdr 1.0

ApplicationWindow {
    width: 800
    height: 675
    visible: true
    title: qsTr("PANDA")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    // Navigation Rail
    Drawer {
        width: 70
        height: parent.height
        id: navRail
        edge: Qt.LeftEdge
        modal: false  // Keep it always open
        interactive: false // prevents from closing when click away
        Component.onCompleted: navRail.open()

        contentItem: Rectangle {
            color: "#101510"
            anchors.fill: parent
            Column {
                anchors.fill: parent
                anchors.topMargin: 30
                spacing: 20

                RailButton {
                    text: qsTr("Home")
                    icon: "qrc:/icons/mods.svg"
                    onClicked: console.log("Home clicked")
                }

                RailButton {
                    text: qsTr("About")
                    icon: "qrc:/icons/about.svg"
                }

                // exit button at bottom
                RailButton {
                    text: qsTr("Exit")
                    icon: "qrc:/icons/exit_app.svg"
                    onClicked: Qt.quit()
                }
            }
        }
    }

    // Appbar
    ToolBar {
        id: toolbar
        Material.background: "#f7fbf2"
        width: parent.width - navRail.width
        anchors.right: parent.right
        RowLayout {
            anchors.fill: parent
            spacing: 16

            Item {
                Layout.fillWidth: true
            }

            // Appbar label
            Label {
                id: toolbarLabel
                text: "PANDA Loader"
                font.pixelSize: 14
                color: "#424940"
                Layout.alignment: Qt.AlignHCenter
            }

            Item {
               Layout.fillWidth: true
           }

            // Notification button
            Item {
                Layout.alignment: Qt.AlignRight
                Layout.fillHeight: true
                Layout.rightMargin: 10
                implicitWidth: 40
                ToolButton {
                    id: notif
                    icon.source: "qrc:/icons/notifications.svg"
                    Layout.alignment: Qt.AlignRight
                    width: 40
                    height: 40
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: navRail.open()
                }

                MultiEffect {
                    source: notif
                    anchors.fill: notif
                    colorization: 1.0
                    colorizationColor: "#424940"
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: console.log("Notifications clicked")
                }
            }

        }
    }

    // Maint content
    Pane {
        id: mainContent
        width: parent.width - navRail.width
        height: parent.height - toolbar.height
        Material.background: "#9daa9e"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        padding: 6
        spacing: 12

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
                        model: modController
                        clip: true

                        delegate: Rectangle { // Mod list container
                            id: modPane
                            width: ListView.view.width
                            height: 50

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
                                controller: modController
                                modelObject: model
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
                    targetComponent: modController.currentMod
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
}
