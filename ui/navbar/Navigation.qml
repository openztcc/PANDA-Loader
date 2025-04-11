import QtQuick
import QtQuick.Controls 2.15

Rectangle {
    id: nav
    color: "#101510"
    anchors.fill: parent
    property StackView stack: null
    property var modPage: null
    property var settingsPage: null
    Column {
        anchors.fill: parent
        anchors.topMargin: 30
        spacing: 20

        RailButton {
            text: qsTr("Home")
            icon: "qrc:/icons/mods.svg"
            onClicked: {
                console.log("Home clicked")
                nav.stack.push(nav.modPage)
            }
        }

        RailButton {
            text: qsTr("Settings")
            icon: "qrc:/icons/about.svg"
            onClicked: {
                console.log("Settings clicked")
                nav.stack.push(nav.settingsPage)
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
