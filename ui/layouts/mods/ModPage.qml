import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

pragma ComponentBehavior: Bound

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
            id: bottomPanel
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 0
            spacing: 0


            handle: Rectangle {
                    // TODO: this is not returning desired values. Potentially override signals.
                    function hoverState(onHoverValue, defaultValue) {
                        if (SplitHandle.pressed || SplitHandle.hovered) {
                            return onHoverValue
                        } else {
                            return defaultValue
                        }
                    }

                    id: handleDelegate
                    implicitWidth: hoverState(4, 0)
                    implicitHeight: 4
                    color: hoverState("#FED286", "#627D58")

                    containmentMask: Item {
                        x: (handleDelegate.width - width) / 2
                        width: 4
                        height: bottomPanel.height
                    }
                }

            // (Left pane) mods list
            ModsList {
                id: modList
                Layout.fillWidth: true
                Layout.fillHeight: true

                Connections {
                    target: modController
                    // function onCurrentModChanged() {
                    //     modList.currentMod = modController.currentMod();
                    // }
                }

                SplitView.fillWidth: true
            }

            // mod details
            InfoPane {
                id: infoPane
                Connections {
                    target: modController
                    function onCurrentModChanged() {
                        infoPane.targetComponent = modController.currentMod();
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
