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
        spacing: 0

        // top launch panel
        LaunchArea {

        }

        // Bottom panel
        SplitView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 0
            spacing: 0

            // (Left pane) mods list
            ModsList {
                id: modList
                Layout.fillWidth: true
                Layout.fillHeight: true

                Connections {
                    target: modController
                    function onCurrentModChanged() {
                        modList.currentMod = modController.currentMod;
                    }
                }

                SplitView.fillWidth: true
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
                SplitView.minimumWidth: 300
                SplitView.maximumWidth: 500
                SplitView.preferredWidth: 400
                Layout.alignment: Qt.AlignRight
            }

        }
    }
}
