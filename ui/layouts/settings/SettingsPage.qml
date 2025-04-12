import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

Pane {
    id: mainContent
    property var navRail: null
    height: parent.height
    Material.background: "#9daa9e"
    anchors.fill: parent
    padding: 0
    spacing: 0

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: settingsPane
            Layout.fillHeight: true
            Layout.preferredWidth: 200

            color: "#2c6431"

            Column {
                anchors.fill: parent
                spacing: 0

                SettingsButton {
                    text: "PANDA Settings"
                    source: "qrc:/icons/paw.svg"
                }

                SettingsButton {
                    text: "Video"
                    source: "qrc:/icons/tv.svg"
                }

                SettingsButton {
                    text: "Sound"
                    source: "qrc:/icons/sound.svg"
                }

                SettingsButton {
                    text: "Gameplay"
                    source: "qrc:/icons/keyboard.svg"
                }

                SettingsButton {
                    text: "Scenarios"
                    source: "qrc:/icons/article.svg"
                }

                SettingsButton {
                    text: "Resource Paths"
                    source: "qrc:/icons/folder.svg"
                }

                SettingsButton {
                    text: "Advanced"
                    source: "qrc:/icons/plumbing.svg"
                }
            }
            Rectangle {
                color: "#eeeeee"
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
