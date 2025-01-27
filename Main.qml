import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects

ApplicationWindow {
    width: 800
    height: 675
    visible: true
    title: qsTr("PANDA")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    // Navigation Rail
    Drawer {
        width: 80
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

        Column {
            anchors.fill: parent

            Rectangle {
                width: parent.width
                height: 150

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
                            onClicked: console.log("Launch clicked")
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

            // main content area
            RowLayout {
                id: actionBar
                width: parent.width
                height: parent.height
                Material.background: "#f7fbf2"
                anchors.top: bgImage.bottom

                // action bar (add, remove, refresh, filter, search)
                RowLayout {
                    Layout.topMargin: 6
                    Layout.preferredHeight: 30

                    // align top
                    Layout.alignment: Qt.AlignTop
                    // Button {
                    //     text: "Add"
                    //     Material.background: "#376a3e"
                    //     Material.foreground: "#d4e8d1"

                    //     MouseArea {
                    //         anchors.fill: parent
                    //         cursorShape: Qt.PointingHandCursor
                    //         onClicked: console.log("Add clicked")
                    //     }
                    // }

                    ActionButton {
                        icon: "qrc:/icons/add.svg"
                        text: "Add"
                        onClicked: console.log("Add clicked")
                    }

                    ActionButton {
                        icon: "qrc:/icons/delete.svg"
                        text: "Remove"
                        onClicked: console.log("Remove clicked")
                    }
                    
                    ActionButton {
                        icon: "qrc:/icons/refresh.svg"
                        text: "Refresh"
                        onClicked: console.log("Refresh clicked")
                    }

                        ComboBox {
                            id: editableDropdown
                            Layout.preferredHeight: 40
                            Layout.preferredWidth: 100
                            editable: true  // Allows typing new values

                            model: ["Red", "Green", "Blue"]

                            onAccepted: {
                                console.log("User entered:", editableDropdown.currentText)
                            }
                        }

                        TextField {
                            id: searchField
                            placeholderText: "Search"
                            Layout.preferredHeight: 40
                            Layout.preferredWidth: 200
                            Material.background: "#f7fbf2"
                            Material.foreground: "#424940"

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: console.log("Search clicked")
                            }
                        }



                    }

                }

        }

    }
}
