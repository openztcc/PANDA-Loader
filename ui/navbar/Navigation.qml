import QtQuick
import QtQuick.Controls 2.15

Rectangle {
    id: nav
    color: "#101510"
    anchors.fill: parent
    property StackView stack: null
    property Component modPage: null
    property Component settingsPage: null
    Column {
        anchors.fill: parent
        anchors.topMargin: 30
        spacing: 20

        RailButton {
            text: qsTr("Home")
            icon: "qrc:/icons/mods.svg"
            onClicked: {
                console.log("Home clicked")
                if (nav.stack && nav.modPage) {
                    nav.stack.push(nav.modPage)
                } else {
                    console.log("One of stack or settingsPage is null")
                }
            }
        }

        RailButton {
            text: qsTr("Settings")
            icon: "qrc:/icons/about.svg"
            onClicked: {
                console.log("Settings clicked")
                if (nav.stack && nav.settingsPage) {
                    nav.stack.push(nav.settingsPage)
                } else {
                    console.log("One of stack or settingsPage is null")
                }
            }
        }

        // exit button at bottom
        RailButton {
            text: qsTr("Exit")
            icon: "qrc:/icons/exit_app.svg"
            onClicked: Qt.quit()
        }
    }
}
