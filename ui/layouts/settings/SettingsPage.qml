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
            color: Qt.darker(mainContent.mainColor, 1.2)
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


            ColumnLayout {
                id: settingsContent
                anchors.fill: parent
                anchors.topMargin: 10
                spacing: 4

                StackView {
                    id: settingsStack
                    Layout.fillHeight: true
                    Layout.fillWidth: true
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

                        ControlPanel {
                            id: displayControls
                            label: "Display Settings"
                            Layout.fillWidth: true

                            PCheckBox { // fullscreen
                                id: fullscreenCheckBox
                                text: "Fullscreen"
                                Layout.fillWidth: true
                            }
                            PTextField { // screenWidth
                                id: screenWidthField
                                title: "Resolution Width"
                                Layout.fillWidth: true
                                descriptionText: "Width of the screen resolution"
                            }
                            PTextField { // screenHeight
                                id: screenHeightField
                                title: "Resolution Height"
                                Layout.fillWidth: true
                                descriptionText: "Height of the screen resolution"
                            }
                            PTextField { // UpdateRate
                                id: updateRateField
                                title: "Update Rate"
                                Layout.fillWidth: true
                                descriptionText: "Update rate of the game"
                            }
                            PTextField { // DrawRate
                                id: drawRateField
                                title: "Draw Rate"
                                Layout.fillWidth: true
                                descriptionText: "Draw rate of the game"
                            }
                        }

                        ControlPanel { // drawfps
                            id: drawFPSControl
                            label: "Display FPS Counter"
                            showSwitch: true
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
                            label: "Completed Scenarios (To unlock other scenarios)"
                            Layout.preferredHeight: implicitHeight
                            Layout.fillWidth: true
                            enabled: true

                            // PCheckBox { // tutorial
                            //     id: tutorialZooCheckBox
                            //     text: "Completed Tutorial Zoo"
                            // }
                            PCheckBox { // aa - Tutorial 1 - Game Controls
                                id: aaCheckBox
                                text: "Completed Tutorial 1 - Game Controls"
                            }
                            PCheckBox {  // ab - Tutorial 2 - Basic Gameplay
                                id: abCheckBox
                                text: "Completed Tutorial 2 - Basic Gameplay"
                            }
                            PCheckBox { // ac - Tutorial 3 - Making Animals Happy
                                id: acCheckBox
                                text: "Completed Tutorial 3 - Making Animals Happy"
                            }
                            PCheckBox { // ad - Dinosaur Digs Tutorial 1
                                id: adCheckBox
                                text: "Completed Dinosaur Digs: Tutorial 1"
                            }
                            PCheckBox { // ae - Marine Mania: Tutorial 1
                                id: aeCheckBox
                                text: "Completed Marine Mania: Tutorial 1"
                            }
                            PCheckBox { // af - Marine Mania: Tutorial 2
                                id: afCheckBox
                                text: "Completed Marine Mania: Tutorial 2"
                            }
                            PCheckBox { // ag - Marine Mania: Tutorial 3
                                id: agCheckBox
                                text: "Completed Marine Mania: Tutorial 3"
                            }
                            PCheckBox { // ba - Small Zoo (Beginner)
                                id: baCheckBox
                                text: "Completed Small Zoo (Beginner)"
                            }
                            PCheckBox { // bb - Seaside Zoo (Beginner)
                                id: bbCheckBox
                                text: "Completed Seaside Zoo (Beginner)"
                            }
                            PCheckBox { // bc - Forest Zoo (Beginner)
                                id: bcCheckBox
                                text: "Completed Forest Zoo (Beginner)"
                            }
                            PCheckBox { // bd - Holiday Tree Farm (Beginner)
                                id: bdCheckBox
                                text: "Completed Holiday Tree Farm (Beginner)"
                            }
                            PCheckBox { // be - Dinosaur Digs: Ice Age Animal Zoo (Beginner)
                                id: beCheckBox
                                text: "Completed Dinosaur Digs: Ice Age Animal Zoo (Beginner)"
                            }
                            PCheckBox { // bf - Marine Mania: Orca Show (Beginner)
                                id: bfCheckBox
                                text: "Completed Marine Mania: Orca Show (Beginner)"
                            }
                            PCheckBox { // bg - Marine Mania: Seasideville Dolphin Park (Beginner)
                                id: bgCheckBox
                                text: "Completed Marine Mania: Seasideville Dolphin Park (Beginner)"
                            }
                            PCheckBox { // bh - Marine Mania: Shark World (Beginner)
                                id: bhCheckBox
                                text: "Completed Marine Mania: Shark World (Beginner)"
                            }
                            PCheckBox { // bi - Marine Mania: Surf and Turf Zoo (Beginner)
                                id: biCheckBox
                                text: "Completed Marine Mania: Surf and Turf Zoo (Beginner)"
                            }
                            PCheckBox { // ca - Revitalize Burkitsville Zoo (Intermediate)
                                id: caCheckBox
                                text: "Completed Revitalize Burkitsville Zoo (Intermediate)"
                            }
                            PCheckBox { // cb - Inner City Zoo (Intermediate)
                                id: cbCheckBox
                                text: "Completed Inner City Zoo (Intermediate)"
                            }
                            PCheckBox { // cc - Saving the Great Cats (Intermediate)
                                id: ccCheckBox
                                text: "Completed Saving the Great Cats (Intermediate)"
                            }
                            PCheckBox { // cd - Endangered Species Zoo (Intermediate)
                                id: cdCheckBox
                                text: "Completed Endangered Species Zoo (Intermediate)"
                            }
                            PCheckBox { // ce - Arctic Zoo (Intermediate)
                                id: ceCheckBox
                                text: "Completed Arctic Zoo (Intermediate)"
                            }
                            PCheckBox { // cf - Beach Resort Zoo (Intermediate)
                                id: cfCheckBox
                                text: "Completed Beach Resort Zoo (Intermediate)"
                            }
                            PCheckBox { // cg - Dinosaur Digs: Valley of the Dinosaurs (Intermediate)
                                id: cgCheckBox
                                text: "Completed Dinosaur Digs: Valley of the Dinosaurs (Intermediate)"
                            }
                            PCheckBox { // ch - Dinosaur Digs: Jurassic Zoo (Intermediate)
                                id: chCheckBox
                                text: "Completed Dinosaur Digs: Jurassic Zoo (Intermediate)"
                            }
                            PCheckBox { // ci - Marine Mania: Oceans of the Zoo (Intermediate)
                                id: ciCheckBox
                                text: "Completed Marine Mania: Oceans of the Zoo (Intermediate)"
                            }
                            PCheckBox { // cj - Marine Mania: Save the Marine Animals (Intermediate)
                                id: cjCheckBox
                                text: "Completed Marine Mania: Save the Marine Animals (Intermediate)"
                            }
                            PCheckBox { // ck - Marine Mania: Free Admission Zoo (Intermediate)
                                id: ckCheckBox
                                text: "Completed Marine Mania: Free Admission Zoo (Intermediate)"
                            }
                            PCheckBox { // cl - Marine Mania: Aquatic Show Park (Intermediate)
                                id: clCheckBox
                                text: "Completed Marine Mania: Aquatic Show Park (Intermediate)"
                            }
                            PCheckBox { // cm - Dinosaur Digs: Carnivore Zoo (Intermediate)
                                id: cmCheckBox
                                text: "Completed Dinosaur Digs: Carnivore Zoo (Intermediate)"
                            }
                            PCheckBox { // cn - Southeast Asian Zoo (Intermediate)
                                id: cnCheckBox
                                text: "Completed Southeast Asian Zoo (Intermediate)"
                            }
                            PCheckBox { // da - Island Zoo (Advanced)
                                id: daCheckBox
                                text: "Completed Island Zoo (Advanced)"
                            }
                            PCheckBox { // db - African Savannah Zoo (Advanced)
                                id: dbCheckBox
                                text: "Completed African Savannah Zoo (Advanced)"
                            }
                            PCheckBox { // dc - Mountain Zoo (Advanced)
                                id: dcCheckBox
                                text: "Completed Mountain Zoo (Advanced)"
                            }
                            PCheckBox { // dd - Tropical Rainforest Zoo (Advanced)
                                id: ddCheckBox
                                text: "Completed Tropical Rainforest Zoo (Advanced)"
                            }
                            PCheckBox { // de - Dinosaur Digs: Dinosaur Island Research Lab (Advanced)
                                id: deCheckBox
                                text: "Completed Dinosaur Digs: Dinosaur Island Research Lab (Advanced)"
                            }
                            PCheckBox { // df - Marine Mania: Marine Conservation (Advanced)
                                id: dfCheckBox
                                text: "Completed Marine Mania: Marine Conservation (Advanced)"
                            }
                            PCheckBox { // dg - Marine Mania: Save the Zoo (Advanced)
                                id: dgCheckBox
                                text: "Completed Marine Mania: Save the Zoo (Advanced)"
                            }
                            PCheckBox { // dh - Conservation Zoo (Advanced)
                                id: dhCheckBox
                                text: "Completed Conservation Zoo (Advanced)"
                            }
                            PCheckBox { // ea - Paradise Island (Very Advanced)
                                id: eaCheckBox
                                text: "Completed Paradise Island (Very Advanced)"
                            }
                            PCheckBox { // fa - Breeding Giant Pandas (Very Advanced)
                                id: faCheckBox
                                text: "Completed Breeding Giant Pandas (Very Advanced)"
                            }
                            PCheckBox { // fb - Dinosaur Digs: Return to Dinosaur Island Research Lab (Very Advanced)
                                id: fbCheckBox
                                text: "Completed Dinosaur Digs: Return to Dinosaur Island Research Lab (Very Advanced)"
                            }
                            PCheckBox { // ga - Dinosaur Digs: Breeding the T.rex (Very Advanced)
                                id: gaCheckBox
                                text: "Completed Dinosaur Digs: Breeding the T.rex (Very Advanced)"
                            }
                            PCheckBox { // gb - Marine Mania: Giant Marine Park (Very Advanced)
                                id: gbCheckBox
                                text: "Completed Marine Mania: Giant Marine Park (Very Advanced)"
                            }
                            PCheckBox { // gc - Marine Mania: Super Zoo (Very Advanced)
                                id: gcCheckBox
                                text: "Completed Marine Mania: Super Zoo (Very Advanced)"
                            }
                            PCheckBox { // gd - Marine Mania: Ultimate Zoo (Very Advanced)
                                id: gdCheckBox
                                text: "Completed Marine Mania: Ultimate Zoo (Very Advanced)"
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
