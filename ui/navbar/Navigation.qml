import QtQuick

Rectangle {
    color: "#101510"
    anchors.fill: parent
    property var currentPage: null
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
                currentPage.push(modPage)
            }
        }

        RailButton {
            text: qsTr("Settings")
            icon: "qrc:/icons/about.svg"
            onClicked: {
                console.log("Settings clicked")
                currentPage.push(settingsPage)
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
