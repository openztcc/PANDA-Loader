import QtQuick

Rectangle {
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
