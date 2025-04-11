import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

Pane {
    id: mainContent
    property var navRail: null

    Material.background: "#9daa9e"
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    padding: 6
    spacing: 12

    Column {
        Layout.fillHeight: true // remaining height
        Layout.preferredWidth: 200
        anchors.fill: parent

        Button {
            text: "PANDA Settings"
            width: 200
            height: 50

            background: Rectangle {
                radius: 0
                color: hovered ? "#66bb6a" : "#4caf50"
            }

            padding: 0
            flat: true
        }


        Button {
            width: parent.width

            id: debugSettings
            text: "Debug"
        }

        Button {
            width: parent.width

            id: mgrSettings
            text: "Managers"
        }

        Button {
            width: parent.width

            id: langSettings
            text: "Language"
        }

        Button {
            width: parent.width

            id: libSettings
            text: "Libraries"
        }

        Button {
            width: parent.width

            id: resSettings
            text: "Resource Paths"
        }

        Button {
            width: parent.width

            id: videoSettings
            text: "Video"
        }

        Button {
            width: parent.width

            id: soundSettings
            text: "Sound"
        }

        Button {
            width: parent.width

            id: uiSettings
            text: "UI"
        }

        Button {
            width: parent.width

            id: saveSettings
            text: "Game Progress"
        }
    }

}
