import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import ozt.panda.PState 1.0

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
                height: 150
                anchors.top: parent.top

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

            // action bar
            RowLayout {
                id: actionBar
                width: parent.width
                height: parent.height
                Material.background: "#f7fbf2"
                anchors.top: launchArea.bottom


                // action bar (add, remove, refresh, filter, search)
                RowLayout {
                    Layout.topMargin: 6
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 400
                    // align top
                    Layout.alignment: Qt.AlignTop

                    ActionButton {
                        icon: "qrc:/icons/add.svg"
                        text: "Add"
                        Layout.preferredHeight: 40
                        onClicked: console.log("Add clicked")
                    }

                    ActionButton {
                        icon: "qrc:/icons/delete.svg"
                        text: "Remove"
                        Layout.preferredHeight: 40
                        onClicked: console.log("Remove clicked")
                    }

                    ActionButton {
                        icon: "qrc:/icons/refresh.svg"
                        text: "Refresh"
                        Layout.preferredHeight: 40
                        onClicked: console.log("Refresh clicked")
                    }

                    ComboBox {
                        id: editableDropdown
                        Layout.preferredHeight: 40
                        Layout.preferredWidth: 100
                        editable: true  // Allows typing new values

                        model: ["Red", "Green", "Blue"]

                        background: Rectangle {
                            color: "#f7fbf2"
                            radius: 0
                        }

                        onAccepted: {
                            console.log("User entered:", editableDropdown.currentText)
                        }
                    }

                    TextField {
                        id: searchField
                        placeholderText: "Search"
                        Layout.preferredHeight: 40
                        Layout.fillWidth: true
                        Material.background: "#f7fbf2"
                        Material.foreground: "#424940"

                        background: Rectangle {
                            color: "#f7fbf2"
                            radius: 0
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: console.log("Search clicked")
                        }
                    }



                }


            }

            // Rectangle {
            //     id: modListArea
            //     Layout.fillWidth: true
            //     Layout.preferredHeight: 50
            //     anchors.top: actionBar.bottom
            //     color: "#f7fbf2"
            //     radius: 0

            //     Text {
            //         text: "Mods"
            //         anchors.centerIn: parent
            //         color: "#424940"
            //         font.pixelSize: 14
            //     }
            // }

            RowLayout {
                // mods list
                Layout.topMargin: -4
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

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
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                            }

                            Text {
                                text: "Enabled"
                                anchors.right: headCheck.left
                            }

                            CheckBox {
                                id: headCheck
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                Material.accent: "#376a3e"
                                checked: true
                            }
                        }
                    }

                    ListView {                        
                        id: modsList
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        boundsBehavior: Flickable.StopAtBounds
                        model: modController

                        clip: true

                        // model: ListModel {
                        //     ListElement { name: "Mod 1" }
                        //     ListElement { name: "Mod 2" }
                        //     ListElement { name: "Mod 3" }
                        //     ListElement { name: "Mod 4" }
                        //     ListElement { name: "Mod 5" }
                        //     ListElement { name: "Mod 6" }
                        //     ListElement { name: "Mod 7" }
                        //     ListElement { name: "Mod 8" }
                        //     ListElement { name: "Mod 9" }
                        //     ListElement { name: "Mod 10" }
                        //     ListElement { name: "Mod 11" }
                        //     ListElement { name: "Mod 12" }
                        //     ListElement { name: "Mod 13" }
                        //     ListElement { name: "Mod 14" }
                        //     ListElement { name: "Mod 15" }
                        //     ListElement { name: "Mod 16" }
                        //     ListElement { name: "Mod 17" }
                        //     ListElement { name: "Mod 18" }
                        //     ListElement { name: "Mod 19" }
                        //     ListElement { name: "Mod 20" }
                        // }

                        delegate: Rectangle {
                            id: modPane
                            width: ListView.view.width
                            height: 75

                            Rectangle {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                height: 1
                                color: "#c1c9be"
                            }

                            Pane {
                                anchors.fill: parent
                                Material.background:  modArea.containsPress ? Qt.lighter("#f7fbf2", 0.8) : modArea.containsMouse ? Qt.darker("#f7fbf2", 0.01) : "#f7fbf2"
                                padding: 12
                                anchors.bottomMargin: 1

                                signal clicked()

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
                                            text: "Buildings"
                                            font.pixelSize: 10
                                        }

                                        // name of mod
                                        Label {
                                            text: model.modTitle
                                            font.pixelSize: 12
                                            color: "#424940"
                                        }

                                        // author(s)
                                        Label {
                                            text: "by " + model.modAuthor
                                            font.pixelSize: 10
                                        }
                                    }

                                    Item {
                                        Layout.fillWidth: true
                                    }

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
                                            console.log("Checkbox changed:", model.modTitle, checked)
                                        }
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            event.accepted = true;
                                        }
                                        propagateComposedEvents: false
                                    }
                                }
                                MouseArea {
                                    id: modArea
                                    anchors.fill: modMeta
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: console.log("Mod clicked:", model.modTitle)
                                    hoverEnabled: true
                                }

                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                            visible: flickableView.moving || flickableView.dragging
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
                Rectangle {
                    id: modDetails
                    Layout.preferredWidth: 300
                    Layout.fillHeight: true
                    anchors.top: actionBar.bottom
                    color: "#f7fbf2"
                    radius: 0

                    Text {
                        text: "Mod details"
                        anchors.centerIn: parent
                        color: "#424940"
                        font.pixelSize: 14
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
}
