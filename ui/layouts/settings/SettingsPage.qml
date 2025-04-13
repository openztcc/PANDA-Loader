import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

pragma ComponentBehavior: Bound

Pane {
    id: mainContent
    property var mainColor: "#2c6431"
    property var mainTextColor: "#E8E8CF"
    height: parent.height
    Material.background: "#57704E"
    anchors.fill: parent
    padding: 0
    spacing: 0

    function replaceSettingsPane(pane, stack, currentButton) {
        settingsStack.replace(pane)
        if (settingsPane.currentButton && settingsPane.currentButton !== currentButton) {
            settingsPane.currentButton.current = false
        }
        currentButton.current = true
        settingsPane.currentButton = currentButton
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: settingsPane
            Layout.fillHeight: true
            Layout.preferredWidth: 200

            color: mainContent.mainColor

            property var currentButton: settingsButton

            Column {
                anchors.fill: parent
                spacing: 0

                SettingsButton {
                    id: settingsButton
                    text: "PANDA Settings"
                    source: "qrc:/icons/paw.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor

                    onClicked: {
                        replaceSettingsPane(pandaSettings, settingsStack, settingsButton)
                    }
                }

                SettingsButton {
                    id: videoSettingsButton
                    text: "Video"
                    source: "qrc:/icons/tv.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor

                    onClicked: {
                        replaceSettingsPane(videoSettings, settingsStack, videoSettingsButton)
                    }
                }

                SettingsButton {
                    id: soundSettingsButton
                    text: "Sound"
                    source: "qrc:/icons/sound.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor

                    onClicked: {
                        replaceSettingsPane(soundSettings, settingsStack, soundSettingsButton)
                    }
                }

                SettingsButton {
                    id: gameplaySettingsButton
                    text: "Gameplay"
                    source: "qrc:/icons/keyboard.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(gameplaySettings, settingsStack, gameplaySettingsButton)
                    }
                }

                SettingsButton {
                    id: scenariosSettingsButton
                    text: "Scenarios"
                    source: "qrc:/icons/article.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(scenariosSettings, settingsStack, scenariosSettingsButton)
                    }
                }

                SettingsButton {
                    id: resourcePathsButton
                    text: "Resource Paths"
                    source: "qrc:/icons/folder.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(resourcePathsSettings, settingsStack, resourcePathsButton)
                    }
                }

                SettingsButton {
                    id: advancedSettingsButton
                    text: "Advanced"
                    source: "qrc:/icons/plumbing.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(advancedSettings, settingsStack, advancedSettingsButton)
                    }
                }
            }

        }

        Rectangle {
            id: settingsPaneDivider
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 1
            color: "#4d4d4d"
        }

        Pane {
            id: settingsRightPane
            Layout.preferredWidth: parent.width - settingsPane.width - settingsPaneDivider.width
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
            background: {
                color: "#9daa9e"
            }
            opacity: 0.8

            padding: 10

            ColumnLayout {
                id: settingsContent
                anchors.fill: parent
                spacing: 4

                StackView {
                    id: settingsStack
                    anchors.fill: parent
                    initialItem: pandaSettings

                    Component.onCompleted: {
                        replaceSettingsPane(pandaSettings, settingsStack, settingsButton)
                    }
                }

                Component {
                    id: pandaSettings
                    // Forms and fields
                    SettingsPane {
                        id: pandaSettingsPane
                        PTextField {
                            id: homePath
                            title: "PANDA Home Path"
                            Layout.fillWidth: true
                            descriptionText: "Path to the PANDA home directory"
                            isFileBrowser: true
                        }
                        PTextField {
                            id: zooPath
                            title: "Zoo Game Path"
                            Layout.fillWidth: true
                            descriptionText: "Path to the zoo.exe install directory"
                            isFileBrowser: true
                        }
                        ControlPanel {
                            id: isoControls
                            label: "Load game from ISO"
                            showSwitch: true
                            Layout.preferredHeight: 120
                            Layout.fillWidth: true
                            enabled: false
                            
                            PTextField {
                                id: isoPath
                                title: "ISO Path"
                                Layout.fillWidth: true
                                descriptionText: "Path to the ISO in local drive"
                                isFileBrowser: true
                            }

                        }
                    }

                }

                Component {
                    id: videoSettings
                    SettingsPane {
                        id: videoSettingsPane
                        // Group of settings for drawfps, drawfpsx, drawfpsy

                        ControlPanel { // drawfps
                            id: drawFPSControl
                            label: "Display FPS Counter"
                            showSwitch: true
                            Layout.preferredHeight: implicitHeight
                            Layout.fillWidth: true
                            enabled: false

                            PTextField { // drawfpsx
                                id: drawFPSX
                                title: "X Position"
                                Layout.fillWidth: true
                                descriptionText: "X position of the FPS counter"
                            }
                            PTextField { // drawfpsy
                                id: drawFPSY
                                title: "Y Position"
                                Layout.fillWidth: true
                                descriptionText: "Y position of the FPS counter"
                            }
                        }
                    }
                }

                Component {
                    id: soundSettings
                    SettingsPane {
                        id: soundSettingsPane
                        PTextField {
                            id: soundSettingsField
                            title: "Sound Settings"
                            Layout.fillWidth: true
                        }
                    }
                }

                Component {
                    id: gameplaySettings
                    SettingsPane {
                        id: gameplaySettingsPane
                        PTextField { // MSStartingCash
                            id: startingCashField
                            title: "Starting Cash"
                            Layout.fillWidth: true
                            descriptionText: "How much cash to start with in freeform mode"
                        }
                        PTextField { // MSCashIncrement
                            id: cashIncrementField
                            title: "Cash Increment"
                            Layout.fillWidth: true
                            descriptionText: "How much cash to increment by in freeform mode"
                        }

                        PTextField { // MSMinCash
                            id: minCashField
                            title: "Minimum Cash"
                            Layout.fillWidth: true
                            descriptionText: "Minimum cash in freeform mode"
                        }
                        PTextField { // MSMaxCash
                            id: maxCashField
                            title: "Maximum Cash"
                            Layout.fillWidth: true
                            descriptionText: "Maximum cash in freeform mode"
                        }
                    }
                }

                Component {
                    id: scenariosSettings
                    SettingsPane {
                        id: scenariosSettingsPane
                        PCheckBox { // startedFirstTutorial
                            id: zooTycoonCheckBox
                            text: "Started Zoo Tycoon Tutorial"
                        }
                        PCheckBox { // startedDinoTutorial
                            id: dinoDigsCheckBox
                            text: "Started Dino Digs Tutorial"                            
                        }
                        PCheckBox { // startedAquaTutorial
                            id: marineManiaCheckBox
                            text: "Started Marine Mania Tutorial"
                        }
                        ControlPanel {
                            id: scenarioControls
                            label: "Completed Scenarios"
                            Layout.preferredHeight: implicitHeight
                            Layout.fillWidth: true
                            enabled: true

                            PCheckBox { // tutorial
                                id: tutorialZooCheckBox
                                text: "Completed Tutorial Zoo"
                            }
                            PCheckBox {  // ba
                                id: basicZooCheckBox
                                text: "Completed Basic Zoo"
                            }
                        }
                    }
                }

                Component {
                    id: resourcePathsSettings
                    SettingsPane {
                        id: resourcePathsSettingsPane
                        // custom component for resource paths
                        PTextField {
                            id: lastZooPlayedPath
                            title: "Last zoo played"
                            Layout.fillWidth: true
                            isFileBrowser: true
                            descriptionText: "Path to the last zoo played"
                        }
                    }
                }

                Component {
                    id: advancedSettings
                    SettingsPane {
                        id: advancedSettingsPane
                        PTextField {
                            id: advancedSettingsField
                            title: "Advanced Settings"
                            Layout.fillWidth: true
                        }
                    }
                }

            }
        
        }

    }
}
