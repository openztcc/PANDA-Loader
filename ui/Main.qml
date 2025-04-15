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
    property var currentButton

    Material.theme: Material.Dark
    Material.accent: Material.LightGreen

    function replacePage(oldPage, newPage, newButton) {
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

        root.currentButton = newButton

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

    Component.onCompleted: {
        root.currentButton = modButton
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

            Rectangle {
                z: 10
                height: parent.height
                width: 1
                color: Qt.darker(parent.color, 1.2)
                anchors.right: parent.right
            }

            Column {
                anchors.fill: parent
                anchors.topMargin: 39

                RailButton {
                    id: modButton
                    text: qsTr("Mods")
                    icon: "qrc:/icons/mods.svg"
                    selected: root.currentButton == modButton
                    topButton: true
                    onClicked: {
                        replacePage(stack.currentItem, modPage, modButton)
                    }
                }

                RailButton {
                    id: settingsButton
                    text: qsTr("Settings")
                    icon: "qrc:/icons/about.svg"
                    selected: root.currentButton == settingsButton
                    onClicked: {
                        replacePage(stack.currentItem, settingsPage, settingsButton)
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

