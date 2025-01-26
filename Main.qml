import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

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
        Row {
            spacing: 16

            ToolButton {
                onClicked: navRail.open()
            }
            Label {
                text: "PANDA"
                font.pixelSize: 14
                color: "#424940"
            }

            Image {

            }
        }
    }

    Pane {
        id: mainContent
        width: parent.width - navRail.width
        height: parent.height - toolbar.height
        Material.background: Material.LightGreen
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        padding: 6

        Column {
            width: parent.width

            Rectangle {
                Material.background: Material.Green
                width: parent.width
                height: 150
            }

            Rectangle {
                width: parent.width
                height: parent.height
            }
        }

    }
}
