import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

LayoutFrame {
    id: modPage

    ColumnLayout {
        Layout.fillHeight: true // remaining height
        Layout.fillWidth: true
        anchors.fill: parent

        // top launch panel
        LaunchArea {

        }

        // Bottom panel
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // (Left pane) mods list
            ModsList {
                id: modList
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: 300
                Layout.preferredHeight: 600

                Connections {
                    target: modController
                    function onCurrentModChanged() {
                        modList.currentMod = modController.currentMod;
                    }
                }
            }

            // mod details
            InfoPane {
                id: infoPane
                Connections {
                    target: modController
                    function onCurrentModChanged() {
                        infoPane.targetComponent = modController.currentMod;
                    }
                }
            }

        }
    }
}
