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
        spacing: 0

        SettingsButton {
            text: "PANDA Settings"
        }

        SettingsButton {
            text: "Debug"
        }

        SettingsButton {
            text: "Managers"
        }

        SettingsButton {
            text: "Language"
        }

        SettingsButton {
            text: "Libraries"
        }

        SettingsButton {
            text: "Resource Paths"
        }

        SettingsButton {
            text: "Video"
        }

        SettingsButton {

            text: "Sound"
        }

        SettingsButton {
            text: "UI"
        }

        SettingsButton {
            text: "Game Progress"
        }
    }

}
