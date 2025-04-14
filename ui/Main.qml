import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import PandaUI 1.0

pragma ComponentBehavior: Bound


ApplicationWindow {
    id: root
    width: 1050
    height: 675
    visible: true
    title: qsTr("PANDA")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    function replacePage(oldPage, newPage) {
        if (oldPage == newPage) {
            console.log("Old page is the same as new page, not replacing")
            return
        } else if (oldPage) {
            console.log("Popping old page: " + oldPage)
            stack.pop()
        }
        if (newPage) {
            console.log("Pushing new page: " + newPage)
            stack.push(newPage)
        }
    }


    // Declare modals
    SimpleModal {
        id: confirmDialog
    }

    StackView {
        id: stack
        anchors.top: toolbar.bottom
        anchors.left: navRail.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        initialItem: modPage
    }

    // Navigation Rail
    Item {
        id: navRail
        width: 70
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        z: 1

        // ---------------------- Navigation Rail ----------------------
        Rectangle {
            id: nav
            color: "#34472D"
            anchors.fill: parent
            property var stack: null
            property var modPage: null
            property var settingsPage: null
            Column {
                anchors.fill: parent
                anchors.topMargin: 30
                spacing: 20

                RailButton {
                    text: qsTr("Mods")
                    icon: "qrc:/icons/mods.svg"
                    onClicked: {
                        replacePage(stack.currentItem, modPage)
                    }
                }

                RailButton {
                    text: qsTr("Settings")
                    icon: "qrc:/icons/about.svg"
                    onClicked: {
                        replacePage(stack.currentItem, settingsPage)
                    }
                }

                // exit button at bottom
                RailButton {
                    text: qsTr("Exit")
                    icon: "qrc:/icons/exit_app.svg"
                    onClicked: Qt.quit()
                }

                Component {
                    id: modPage
                    ModPage {

                    }
                }

                Component {
                    id: settingsPage
                    SettingsPage { 
                        mainColor: "#77956C"
                    }
                }
            }
        }

    }

    // Appbar
    ToolBar {
        id: toolbar
        anchors.left: navRail.right
        anchors.right: parent.right
        anchors.top: parent.top
        height: 40

        AppBarLayout {
            id: appbar
            anchors.fill: parent
        }
    }


}

