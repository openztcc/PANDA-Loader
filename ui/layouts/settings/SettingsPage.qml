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
    padding: 0
    spacing: 0

    Column {
        Layout.fillHeight: true // remaining height
        Layout.preferredWidth: 200
        width: 200
        anchors.left: parent.left
        spacing: 0
        background: Rectangle {
            color: "#9daa9e"
            radius: 0
        }

        SettingsButton {
            text: "PANDA Settings"
        }

        SettingsButton {
            text: "Video"
        }

        SettingsButton {
            text: "Sound"
        }

        SettingsButton {
            text: "Gameplay"
        }

        SettingsButton {
            text: "Scenarios"
        }

        SettingsButton {
            text: "Resource Paths"
        }

        SettingsButton {
            text: "Advanced"
        }
    }

}
