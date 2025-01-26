import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("PANDA")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

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

    Pane {
        id: mainContent
        width: parent.width - navRail.width
        height: parent.height
        Material.background: Material.LightGreen
        anchors.right: parent.right

        Column {
            width: parent.width
            ToolBar {
                Material.background: Material.Light
                width: parent.width
                Row {
                    spacing: 16
                    ToolButton {
                        onClicked: navRail.open()
                    }
                    Label {
                        text: "PANDA"
                        font.pixelSize: 24
                    }
                }
            }

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
