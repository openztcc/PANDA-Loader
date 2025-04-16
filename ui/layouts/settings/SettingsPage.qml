import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

pragma ComponentBehavior: Bound

LayoutFrame {
    id: mainContent
    property var mainColor: "#2c6431"
    property var mainTextColor: "#E8E8CF"
    property var rightPaneColor: "#57704E"
    Layout.fillWidth: true
    Layout.fillHeight: true

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
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 0

        Rectangle {
            id: settingsPane
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            Layout.alignment: Qt.AlignLeft

            color: mainContent.mainColor

            property var currentButton: settingsButton

            Rectangle {
                z: 1
                color: Qt.darker(mainContent.rightPaneColor, 1.2)
                width: 1
                height: parent.height
                anchors.right: parent.right
            }



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

                SettingsButton  {
                    id: uiSettingsButton
                    text: "UI"
                    source: "qrc:/icons/ui.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(uiSettings, settingsStack, uiSettingsButton)
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
                    id: languageSettingsButton
                    text: "Language"
                    source: "qrc:/icons/language.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(languageSettings, settingsStack, languageSettingsButton)
                    }
                }

                SettingsButton {
                    id: debugSettingsButton
                    text: "Debug Settings"
                    source: "qrc:/icons/plumbing.svg"
                    color: mainContent.mainColor
                    fg: mainContent.mainTextColor
                    onClicked: {
                        replaceSettingsPane(debugSettings, settingsStack, debugSettingsButton)
                    }
                }
            }
        }

        Pane {
            id: settingsRightPane
            Layout.preferredWidth: parent.width - settingsPane.width
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
            Layout.fillWidth: true
            Material.background: mainContent.rightPaneColor            

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
                            text: psettings.pandaHomePath()
                        }
                        PTextField {
                            id: zooPath
                            title: "Zoo Game Path"
                            Layout.fillWidth: true
                            descriptionText: "Path to the zoo.exe install directory"
                            isFileBrowser: true
                            text: psettings.zooGamePath()
                        }
                        ControlPanel {
                            id: isoControls
                            label: "Load game from ISO"
                            showSwitch: true
                            Layout.preferredHeight: 120
                            Layout.fillWidth: true
                            checked: psettings.useIsoMounting()

                            PTextField {
                                id: isoPath
                                title: "ISO Path"
                                Layout.fillWidth: true
                                descriptionText: "Path to the ISO in local drive"
                                isFileBrowser: true
                                text: psettings.isoPath()
                            }

                        }
                    }

                }

                Component {
                    id: videoSettings
                    SettingsPane {
                        id: videoSettingsPane
                        // Group of settings for drawfps, drawfpsx, drawfpsy

                        PCheckBox { // loadHalfAnims
                            id: loadHalfAnimsCheckBox
                            text: "Load Half Animations"
                            checked: zoo.getBool("advanced", "loadHalfAnims")
                        }

                        ControlPanel {
                            id: displayControls
                            label: "Display Settings"
                            Layout.fillWidth: true

                            PCheckBox { // fullscreen
                                id: fullscreenCheckBox
                                text: "Fullscreen"
                                Layout.fillWidth: true
                                checked: zoo.getBool("user", "fullscreen")
                            }
                            PTextField { // screenWidth
                                id: screenWidthField
                                title: "Resolution Width"
                                Layout.fillWidth: true
                                descriptionText: "Width of the screen resolution"
                                text: zoo.getString("user", "screenwidth")
                            }
                            PTextField { // screenHeight
                                id: screenHeightField
                                title: "Resolution Height"
                                Layout.fillWidth: true
                                descriptionText: "Height of the screen resolution"
                                text: zoo.getString("user", "screenheight")
                            }
                            PTextField { // UpdateRate
                                id: updateRateField
                                title: "Update Rate"
                                Layout.fillWidth: true
                                descriptionText: "Update rate of the game"
                                text: zoo.getString("user", "UpdateRate")
                            }
                            PTextField { // DrawRate
                                id: drawRateField
                                title: "Draw Rate"
                                Layout.fillWidth: true
                                descriptionText: "Draw rate of the game"
                                text: zoo.getString("user", "DrawRate")
                            }
                        }

                        ControlPanel { // drawfps
                            id: drawFPSControl
                            label: "Display FPS Counter"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: zoo.getBool("debug", "drawfps")

                            PTextField { // drawfpsx
                                id: drawFPSX
                                title: "X Position"
                                Layout.fillWidth: true
                                descriptionText: "X position of the FPS counter"
                                text: zoo.getString("debug", "drawfpsx")
                            }
                            PTextField { // drawfpsy
                                id: drawFPSY
                                title: "Y Position"
                                Layout.fillWidth: true
                                descriptionText: "Y position of the FPS counter"
                                text: zoo.getString("debug", "drawfpsy")
                            }
                        }
                    }
                }

                Component {
                    id: soundSettings
                    SettingsPane {
                        id: soundSettingsPane
                        PCheckBox { // use8BitSound
                            id: use8BitSoundCheckBox
                            text: "Use 8 Bit Sound"
                            checked: zoo.getBool("advanced", "use8BitSound")
                        }
                        PTextField { // userAttenuation 
                            id: userAttenuationTextField
                            title: "User Attenuation"
                            Layout.fillWidth: true
                            text: zoo.getString("UI", "userAttenuation")
                        }
                        PTextField { // completedExhibitAttenuation
                            id: completedExhibitAttenuationTextField
                            title: "Completed Exhibit Attenuation"
                            Layout.fillWidth: true
                            text: zoo.getString("UI", "completedExhibitAttenuation")
                        }

                        ControlPanel { // menu music controls
                            id: menuMusicControls
                            label: "Menu Music Enabled"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false
                            PTextField { // menuMusicAttenuation
                                id: menuMusicAttenuationField
                                title: "Menu Music Volume"
                                Layout.fillWidth: true
                                text: zoo.getString("UI", "menuMusicAttenuation")
                            }
                            PTextField { // menuMusic 
                                id: menuMusicField
                                title: "Menu Music"
                                Layout.fillWidth: true
                                isFileBrowser: true
                                descriptionText: "Path to the menu music file"
                                text: zoo.getString("UI", "menuMusic")
                            }
                        }
                        ControlPanel { // boot music controls
                            id: bootMusicControls
                            label: "Boot Music Volume"
                            Layout.fillWidth: true
                            PTextField { // movevolume1
                                id: moveVolume1Field
                                title: "Volume for Splash Screen 1"
                                Layout.fillWidth: true
                                descriptionText: "Volume for the first splash screen that plays"
                                text: zoo.getString("UI", "movievolume1")
                            }
                            PTextField { // movevolume2
                                id: moveVolume2Field
                                title: "Volume for Splash Screen 2"
                                Layout.fillWidth: true
                                descriptionText: "Volume for the second splash screen that plays"
                                text: zoo.getString("UI", "movievolume2")
                            }
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
                            text: zoo.getString("UI", "MSStartingCash")   
                        }
                        PTextField { // MSCashIncrement
                            id: cashIncrementField
                            title: "Cash Increment"
                            Layout.fillWidth: true
                            descriptionText: "How much cash to increment by in freeform mode"
                            text: zoo.getString("UI", "MSCashIncrement")
                        }

                        PTextField { // MSMinCash
                            id: minCashField
                            title: "Minimum Cash"
                            Layout.fillWidth: true
                            descriptionText: "Minimum cash in freeform mode"
                            text: zoo.getString("UI", "MSMinCash")
                        }
                        PTextField { // MSMaxCash
                            id: maxCashField
                            title: "Maximum Cash"
                            Layout.fillWidth: true
                            descriptionText: "Maximum cash in freeform mode"
                            text: zoo.getString("UI", "MSMaxCash")
                        }
                        ControlPanel { // mouse controls 
                            id: controlsSettings
                            label: "Mouse Settings"
                            Layout.fillWidth: true

                            PTextField { // drag 
                                id: dragField
                                title: "Drag"
                                Layout.fillWidth: true
                                text: zoo.getString("advanced", "drag")
                            }

                            PTextField { // Click
                                id: clickField
                                title: "Click"
                                Layout.fillWidth: true
                                text: zoo.getString("advanced", "click")
                            }

                            PTextField { // normal
                                id: normalField
                                title: "Normal"
                                Layout.fillWidth: true
                                text: zoo.getString("advanced", "normal")
                            }

                            PTextField { // level 
                                id: levelField
                                title: "Level"
                                Layout.fillWidth: true
                                text: zoo.getString("advanced", "level")
                            }

                            PTextField { // mouseScrollThreshold
                                id: mouseScrollThresholdField
                                title: "Mouse Scroll Threshold"
                                Layout.fillWidth: true
                                descriptionText: "Threshold for mouse scroll"
                                text: zoo.getString("UI", "mouseScrollThreshold")
                            }

                            PTextField { // mouseScrollDelay
                                id: mouseScrollDelayField
                                title: "Mouse Scroll Delay"
                                Layout.fillWidth: true
                                descriptionText: "Delay for mouse scroll"
                                text: zoo.getString("UI", "mouseScrollDelay")
                            }

                            PTextField { // mouseScrollX
                                id: mouseScrollXField
                                title: "Mouse Scroll X"
                                Layout.fillWidth: true
                                descriptionText: "X rate of mouse scroll"
                                text: zoo.getString("UI", "mouseScrollX")
                            }

                            PTextField { // mouseScrollY
                                id: mouseScrollYField
                                title: "Mouse Scroll Y"
                                Layout.fillWidth: true
                                descriptionText: "Y rate of mouse scroll"
                                text: zoo.getString("UI", "mouseScrollY")
                            }
                        }

                        ControlPanel { // key controls 
                            id: keyControlsSettings
                            label: "Keyboard Settings"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false

                            PTextField { // keyScrollX
                                id: keyScrollXField
                                title: "Key Scroll X"
                                Layout.fillWidth: true
                                descriptionText: "X rate of x-directional panning"
                                text: zoo.getString("UI", "keyScrollX")
                            }

                            PTextField { // keyScrollY
                                id: keyScrollYField
                                title: "Key Scroll Y"
                                Layout.fillWidth: true
                                descriptionText: "Y rate of y-directional panning"
                                text: zoo.getString("UI", "keyScrollY")
                            }
                        }

                        ControlPanel { // map size
                            id: mapSizeControls
                            label: "Map Size"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false

                            PTextField { // mapX
                                id: mapXField
                                title: "Map X"
                                Layout.fillWidth: true
                                descriptionText: "X size of the map (note: this does not change the map size)"
                                text: zoo.getString("Map", "mapX")
                            }

                            PTextField { // mapY
                                id: mapYField
                                title: "Map Y"
                                Layout.fillWidth: true
                                descriptionText: "Y size of the map (note: this does not change the map size)"
                                text: zoo.getString("Map", "mapY")
                            }
                        }
                    }
                }

                Component {
                    id: uiSettings
                    SettingsPane {
                        id: uiSettingsPane

                        PTextField { // defaultEditCharLimit
                            id: defaultEditCharLimitField
                            title: "Default Edit Character Limit"
                            Layout.fillWidth: true
                            descriptionText: "Default character limit for edit fields"
                            text: zoo.getString("UI", "defaultEditCharLimit")
                        }

                        ControlPanel { // Tooltip delay
                            id: tooltipDelayControl
                            label: "Tooltip Delay"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: zoo.getBool("UI", "tooltipDelay")

                            PTextField { // tooltipDuration
                                id: tooltipDurationField
                                title: "Tooltip Duration"
                                Layout.fillWidth: true
                                descriptionText: "Duration of the tooltip"
                                text: zoo.getString("UI", "tooltipDuration")
                            }
                        }

                        ControlPanel { // Tooltip Widths
                            id: tooltipWidthControl
                            label: "Tooltip Widths"
                            Layout.fillWidth: true

                            PTextField { // maxShortTooltipWidth
                                id: maxShortTooltipWidthField
                                title: "Max Short Tooltip Width"
                                Layout.fillWidth: true
                                descriptionText: "Maximum width of short tooltips"
                                text: zoo.getString("UI", "maxShortTooltipWidth")
                            }

                            PTextField { // maxLongTooltipWidth
                                id: maxLongTooltipWidthField
                                title: "Max Long Tooltip Width"
                                Layout.fillWidth: true
                                descriptionText: "Maximum width of long tooltips"
                                text: zoo.getString("UI", "maxLongTooltipWidth")
                            }
                        }

                        PTextField { // helpType
                            id: helpTypeField
                            title: "Help Type"
                            Layout.fillWidth: true
                            text: zoo.getString("UI", "helpType")
                        }

                        ControlPanel { // MessageDisplay
                            id: messageDisplayControl
                            label: "Message Display"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: zoo.getBool("UI", "MessageDisplay")

                            PTextField { // minimumMessageInterval
                                id: minimumMessageIntervalField
                                title: "Minimum Message Interval"
                                Layout.fillWidth: true
                                descriptionText: "Minimum interval between messages"
                                text: zoo.getString("UI", "minimumMessageInterval")
                            }
                        }

                        ControlPanel { // progress bar location
                            id: progressBarControls
                            label: "Progress Bar Settings"
                            Layout.fillWidth: true

                            PTextField { // progressLeft
                                id: progressBarLeftField
                                title: "Left"
                                Layout.fillWidth: true
                                descriptionText: "Left position of the progress bar"
                                text: zoo.getString("UI", "progressLeft")
                            }
                            PTextField { // progressTop
                                id: progressBarTopField
                                title: "Top"
                                Layout.fillWidth: true
                                descriptionText: "Top position of the progress bar"
                                text: zoo.getString("UI", "progressTop")
                            }
                            PTextField { // progressBottom
                                id: progressBarBottomField
                                title: "Bottom"
                                Layout.fillWidth: true
                                descriptionText: "Bottom position of the progress bar"
                                text: zoo.getString("UI", "progressBottom")
                            }
                            PTextField { // progressRight
                                id: progressBarRightField
                                title: "Right"
                                Layout.fillWidth: true
                                descriptionText: "Right position of the progress bar"
                                text: zoo.getString("UI", "progressRight")
                            }
                        }

                        ControlPanel { // progress bar color
                            id: progressBarColorControls
                            label: "Progress Bar Color"
                            Layout.fillWidth: true

                            PTextField { // progressRed
                                id: progressBarRedField
                                title: "Red"
                                Layout.fillWidth: true
                                descriptionText: "Red color value of the progress bar"
                                text: zoo.getString("UI", "progressRed")
                            }

                            PTextField { // progressGreen
                                id: progressBarGreenField
                                title: "Green"
                                Layout.fillWidth: true
                                descriptionText: "Green color value of the progress bar"
                                text: zoo.getString("UI", "progressGreen")
                            }
                            PTextField { // progressBlue
                                id: progressBarBlueField
                                title: "Blue"
                                Layout.fillWidth: true
                                descriptionText: "Blue color value of the progress bar"
                                text: zoo.getString("UI", "progressBlue")
                            }

                        }

                        ControlPanel { // progress bar shadow offset
                            id: progressBarShadowControls
                            label: "Progress Bar Shadow Offset"
                            Layout.fillWidth: true

                            PTextField { //progressShadowXOffset 
                                id: progressBarShadowXOffsetTextField
                                title: "X Offset"
                                Layout.fillWidth: true
                                descriptionText: "X offset of the progress bar shadow"
                                text: zoo.getString("UI", "progressShadowXOffset")
                            }
                            PTextField { // progressShadowYOffset
                                id: progressBarShadowYOffsetTextField
                                title: "Y Offset"
                                Layout.fillWidth: true
                                descriptionText: "Y offset of the progress bar shadow"
                                text: zoo.getString("UI", "progressShadowYOffset")
                            }
                        }

                        ControlPanel { // progress bar shadow color
                            id: progressBarShadowColorControls
                            label: "Progress Bar Shadow Color"
                            Layout.fillWidth: true

                            PTextField { // progressShadowRed
                                id: progressBarShadowRedField
                                title: "Red"
                                Layout.fillWidth: true
                                descriptionText: "Red color value of the progress bar shadow"
                                text: zoo.getString("UI", "progressShadowRed")
                            }

                            PTextField { // progressShadowGreen
                                id: progressBarShadowGreenField
                                title: "Green"
                                Layout.fillWidth: true
                                descriptionText: "Green color value of the progress bar shadow"
                                text: zoo.getString("UI", "progressShadowGreen")
                            }
                            PTextField { // progressShadowBlue
                                id: progressBarShadowBlueField
                                title: "Blue"
                                Layout.fillWidth: true
                                descriptionText: "Blue color value of the progress bar shadow"
                                text: zoo.getString("UI", "progressShadowBlue")
                            }
                        }

                        ControlPanel { // last window position
                            id: lastWindowPositionControls
                            label: "Last Window Position"
                            Layout.fillWidth: true

                            PTextField { // lastWindowX
                                id: lastWindowXField
                                title: "Last Window X"
                                Layout.fillWidth: true
                                descriptionText: "Relative to the main monitor, X position where the game was last closed"
                                text: zoo.getString("UI", "lastWindowX")
                            }
                            PTextField { // lastWindowY
                                id: lastWindowYField
                                title: "Last Window Y"
                                Layout.fillWidth: true
                                descriptionText: "Relative to the main monitor, Y position where the game was last closed"
                                text: zoo.getString("UI", "lastWindowY")
                            }        
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
                            checked: zoo.getBool("UI", "startedFirstTutorial")
                        }
                        PCheckBox { // startedDinoTutorial
                            id: dinoDigsCheckBox
                            text: "Started Dino Digs Tutorial"
                            checked: zoo.getBool("UI", "startedDinoTutorial")
                        }
                        PCheckBox { // startedAquaTutorial
                            id: marineManiaCheckBox
                            text: "Started Marine Mania Tutorial"
                            checked: zoo.getBool("UI", "startedAquaTutorial")
                        }
                        ControlPanel {
                            id: scenarioControls
                            label: "Completed Scenarios (To unlock other scenarios)"
                            Layout.preferredHeight: implicitHeight
                            Layout.fillWidth: true
                            checked: true

                            // PCheckBox { // tutorial
                            //     id: tutorialZooCheckBox
                            //     text: "Completed Tutorial Zoo"
                            // }
                            PCheckBox { // aa - Tutorial 1 - Game Controls
                                id: aaCheckBox
                                text: "Completed Tutorial 1 - Game Controls"
                                checked: zoo.getBool("scenario", "aa")
                            }
                            PCheckBox {  // ab - Tutorial 2 - Basic Gameplay
                                id: abCheckBox
                                text: "Completed Tutorial 2 - Basic Gameplay"
                                checked: zoo.getBool("scenario", "ab")
                            }
                            PCheckBox { // ac - Tutorial 3 - Making Animals Happy
                                id: acCheckBox
                                text: "Completed Tutorial 3 - Making Animals Happy"
                                checked: zoo.getBool("scenario", "ac")
                            }
                            PCheckBox { // ad - Dinosaur Digs Tutorial 1
                                id: adCheckBox
                                text: "Completed Dinosaur Digs: Tutorial 1"
                                checked: zoo.getBool("scenario", "ad")
                            }
                            PCheckBox { // ae - Marine Mania: Tutorial 1
                                id: aeCheckBox
                                text: "Completed Marine Mania: Tutorial 1"
                                checked: zoo.getBool("scenario", "ae")
                            }
                            PCheckBox { // af - Marine Mania: Tutorial 2
                                id: afCheckBox
                                text: "Completed Marine Mania: Tutorial 2"
                                checked: zoo.getBool("scenario", "af")
                            }
                            PCheckBox { // ag - Marine Mania: Tutorial 3
                                id: agCheckBox
                                text: "Completed Marine Mania: Tutorial 3"
                                checked: zoo.getBool("scenario", "ag")
                            }
                            PCheckBox { // ba - Small Zoo (Beginner)
                                id: baCheckBox
                                text: "Completed Small Zoo (Beginner)"
                                checked: zoo.getBool("scenario", "ba")
                            }
                            PCheckBox { // bb - Seaside Zoo (Beginner)
                                id: bbCheckBox
                                text: "Completed Seaside Zoo (Beginner)"
                                checked: zoo.getBool("scenario", "bb")
                            }
                            PCheckBox { // bc - Forest Zoo (Beginner)
                                id: bcCheckBox
                                text: "Completed Forest Zoo (Beginner)"
                                checked: zoo.getBool("scenario", "bc")
                            }
                            PCheckBox { // bd - Holiday Tree Farm (Beginner)
                                id: bdCheckBox
                                text: "Completed Holiday Tree Farm (Beginner)"
                                checked: zoo.getBool("scenario", "bd")
                            }
                            PCheckBox { // be - Dinosaur Digs: Ice Age Animal Zoo (Beginner)
                                id: beCheckBox
                                text: "Completed Dinosaur Digs: Ice Age Animal Zoo (Beginner)"
                                checked: zoo.getBool("scenario", "be")
                            }
                            PCheckBox { // bf - Marine Mania: Orca Show (Beginner)
                                id: bfCheckBox
                                text: "Completed Marine Mania: Orca Show (Beginner)"
                                checked: zoo.getBool("scenario", "bf")
                            }
                            PCheckBox { // bg - Marine Mania: Seasideville Dolphin Park (Beginner)
                                id: bgCheckBox
                                text: "Completed Marine Mania: Seasideville Dolphin Park (Beginner)"
                                checked: zoo.getBool("scenario", "bg")
                            }
                            PCheckBox { // bh - Marine Mania: Shark World (Beginner)
                                id: bhCheckBox
                                text: "Completed Marine Mania: Shark World (Beginner)"
                                checked: zoo.getBool("scenario", "bh")
                            }
                            PCheckBox { // bi - Marine Mania: Surf and Turf Zoo (Beginner)
                                id: biCheckBox
                                text: "Completed Marine Mania: Surf and Turf Zoo (Beginner)"
                                checked: zoo.getBool("scenario", "bi")
                            }
                            PCheckBox { // ca - Revitalize Burkitsville Zoo (Intermediate)
                                id: caCheckBox
                                text: "Completed Revitalize Burkitsville Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "ca")
                            }
                            PCheckBox { // cb - Inner City Zoo (Intermediate)
                                id: cbCheckBox
                                text: "Completed Inner City Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "cb")
                            }
                            PCheckBox { // cc - Saving the Great Cats (Intermediate)
                                id: ccCheckBox
                                text: "Completed Saving the Great Cats (Intermediate)"
                                checked: zoo.getBool("scenario", "cc")
                            }
                            PCheckBox { // cd - Endangered Species Zoo (Intermediate)
                                id: cdCheckBox
                                text: "Completed Endangered Species Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "cd")
                            }
                            PCheckBox { // ce - Arctic Zoo (Intermediate)
                                id: ceCheckBox
                                text: "Completed Arctic Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "ce")
                            }
                            PCheckBox { // cf - Beach Resort Zoo (Intermediate)
                                id: cfCheckBox
                                text: "Completed Beach Resort Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "cf")
                            }
                            PCheckBox { // cg - Dinosaur Digs: Valley of the Dinosaurs (Intermediate)
                                id: cgCheckBox
                                text: "Completed Dinosaur Digs: Valley of the Dinosaurs (Intermediate)"
                                checked: zoo.getBool("scenario", "cg")
                            }
                            PCheckBox { // ch - Dinosaur Digs: Jurassic Zoo (Intermediate)
                                id: chCheckBox
                                text: "Completed Dinosaur Digs: Jurassic Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "ch")
                            }
                            PCheckBox { // ci - Marine Mania: Oceans of the Zoo (Intermediate)
                                id: ciCheckBox
                                text: "Completed Marine Mania: Oceans of the Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "ci")
                            }
                            PCheckBox { // cj - Marine Mania: Save the Marine Animals (Intermediate)
                                id: cjCheckBox
                                text: "Completed Marine Mania: Save the Marine Animals (Intermediate)"
                                checked: zoo.getBool("scenario", "cj")
                            }
                            PCheckBox { // ck - Marine Mania: Free Admission Zoo (Intermediate)
                                id: ckCheckBox
                                text: "Completed Marine Mania: Free Admission Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "ck")
                            }
                            PCheckBox { // cl - Marine Mania: Aquatic Show Park (Intermediate)
                                id: clCheckBox
                                text: "Completed Marine Mania: Aquatic Show Park (Intermediate)"
                                checked: zoo.getBool("scenario", "cl")
                            }
                            PCheckBox { // cm - Dinosaur Digs: Carnivore Zoo (Intermediate)
                                id: cmCheckBox
                                text: "Completed Dinosaur Digs: Carnivore Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "cm")
                            }
                            PCheckBox { // cn - Southeast Asian Zoo (Intermediate)
                                id: cnCheckBox
                                text: "Completed Southeast Asian Zoo (Intermediate)"
                                checked: zoo.getBool("scenario", "cn")
                            }
                            PCheckBox { // da - Island Zoo (Advanced)
                                id: daCheckBox
                                text: "Completed Island Zoo (Advanced)"
                                checked: zoo.getBool("scenario", "da")
                            }
                            PCheckBox { // db - African Savannah Zoo (Advanced)
                                id: dbCheckBox
                                text: "Completed African Savannah Zoo (Advanced)"
                                checked: zoo.getBool("scenario", "db")
                            }
                            PCheckBox { // dc - Mountain Zoo (Advanced)
                                id: dcCheckBox
                                text: "Completed Mountain Zoo (Advanced)"
                                checked: zoo.getBool("scenario", "dc")
                            }
                            PCheckBox { // dd - Tropical Rainforest Zoo (Advanced)
                                id: ddCheckBox
                                text: "Completed Tropical Rainforest Zoo (Advanced)"
                                checked: zoo.getBool("scenario", "dd")
                            }
                            PCheckBox { // de - Dinosaur Digs: Dinosaur Island Research Lab (Advanced)
                                id: deCheckBox
                                text: "Completed Dinosaur Digs: Dinosaur Island Research Lab (Advanced)"
                                checked: zoo.getBool("scenario", "de")
                            }
                            PCheckBox { // df - Marine Mania: Marine Conservation (Advanced)
                                id: dfCheckBox
                                text: "Completed Marine Mania: Marine Conservation (Advanced)"
                                checked: zoo.getBool("scenario", "df")
                            }
                            PCheckBox { // dg - Marine Mania: Save the Zoo (Advanced)
                                id: dgCheckBox
                                text: "Completed Marine Mania: Save the Zoo (Advanced)"
                                checked: zoo.getBool("scenario", "dg")
                            }
                            PCheckBox { // dh - Conservation Zoo (Advanced)
                                id: dhCheckBox
                                text: "Completed Conservation Zoo (Advanced)"
                                checked: zoo.getBool("scenario", "dh")
                            }
                            PCheckBox { // ea - Paradise Island (Very Advanced)
                                id: eaCheckBox
                                text: "Completed Paradise Island (Very Advanced)"
                                checked: zoo.getBool("scenario", "ea")
                            }
                            PCheckBox { // fa - Breeding Giant Pandas (Very Advanced)
                                id: faCheckBox
                                text: "Completed Breeding Giant Pandas (Very Advanced)"
                                checked: zoo.getBool("scenario", "fa")
                            }
                            PCheckBox { // fb - Dinosaur Digs: Return to Dinosaur Island Research Lab (Very Advanced)
                                id: fbCheckBox
                                text: "Completed Dinosaur Digs: Return to Dinosaur Island Research Lab (Very Advanced)"
                                checked: zoo.getBool("scenario", "fb")
                            }
                            PCheckBox { // ga - Dinosaur Digs: Breeding the T.rex (Very Advanced)
                                id: gaCheckBox
                                text: "Completed Dinosaur Digs: Breeding the T.rex (Very Advanced)"
                                checked: zoo.getBool("scenario", "ga")
                            }
                            PCheckBox { // gb - Marine Mania: Giant Marine Park (Very Advanced)
                                id: gbCheckBox
                                text: "Completed Marine Mania: Giant Marine Park (Very Advanced)"
                                checked: zoo.getBool("scenario", "gb")
                            }
                            PCheckBox { // gc - Marine Mania: Super Zoo (Very Advanced)
                                id: gcCheckBox
                                text: "Completed Marine Mania: Super Zoo (Very Advanced)"
                                checked: zoo.getBool("scenario", "gc")
                            }
                            PCheckBox { // gd - Marine Mania: Ultimate Zoo (Very Advanced)
                                id: gdCheckBox
                                text: "Completed Marine Mania: Ultimate Zoo (Very Advanced)"
                                checked: zoo.getBool("scenario", "gd")
                            }
                        }
                    }
                }

                Component {
                    id: resourcePathsSettings
                    SettingsPane {
                        id: resourcePathsSettingsPane
                        // custom component for resource paths
                        PTextField { // lastfile
                            id: lastZooPlayedPath
                            title: "Last Zoo Played"
                            Layout.fillWidth: true
                            isFileBrowser: true
                            descriptionText: "Path to the last zoo played"
                            text: zoo.getString("user", "lastfile")
                        }
                        // resource paths
                        AppendTable {
                            id: pathTable
                            model: pathModel
                            Layout.fillWidth: true
                        }
                        ControlPanel {
                            id: advancedResControls
                            label: "Advanced Resources"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false
                            PTextField { // aimgr
                                id: aimgrPath
                                title: "aimgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "aimgr")
                            }
                            PTextField { // worldmgr
                                id: worldmgrPath
                                title: "worldmgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "worldmgr")
                            }
                            PTextField { // gamemgr
                                id: gamemgrPath
                                title: "gamemgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "gamemgr")
                            }
                            PTextField { // scenariomgr
                                id: scenarioMgrPath
                                title: "scenarioMgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "scenariomgr")
                            }
                            PTextField { // scriptmgr
                                id: scriptMgrPath
                                title: "scriptMgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "scriptmgr")
                            }
                            PTextField { // soundmgr
                                id: soundMgrPath
                                title: "soundMgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "soundmgr")
                            }
                            PTextField { // terrainmgr
                                id: terrainMgrPath
                                title: "terrainMgr"
                                Layout.fillWidth: true
                                text: zoo.getString("mgr", "terrainmgr")
                            }
                            PTextField { // res
                                id: resPath
                                title: "res"
                                Layout.fillWidth: true
                                text: zoo.getString("lib", "res")
                            }
                            PTextField { // lang
                                id: langPath
                                title: "lang"
                                Layout.fillWidth: true
                                text: zoo.getString("lib", "lang")
                            }
                        }
                    }
                }

                Component {
                    id: languageSettings
                    SettingsPane {
                        id: languageSettingsPane
                        PTextField { // lang
                            id: languageField
                            title: "Language"
                            Layout.fillWidth: true
                            text: zoo.getString("language", "lang")
                        }
                        PTextField { // sublang
                            id: subLanguageField
                            title: "Sub Language"
                            Layout.fillWidth: true
                            text: zoo.getString("language", "sublang")
                        }
                    }
                }

                Component {
                    id: debugSettings
                    SettingsPane {
                        id: debugSettingsPane
                        PCheckBox {
                            id: devModeCheckBox
                            text: "Developer Mode Enabled"
                            checked: zoo.getBool("debug", "devModeEnabled")
                        }
                        ControlPanel {
                            id: debugControls
                            label: "Debug Settings (Applies to placed entities)"
                            Layout.fillWidth: true

                            PCheckBox { // ShowAIInfo
                                id: showAIInfoCheckBox
                                text: "Show AI Info"
                                checked: zoo.getBool("debug", "ShowAIInfo")
                            }
                            PCheckBox { // ShowName
                                id: showNameCheckBox
                                text: "Show Names"
                                checked: zoo.getBool("debug", "ShowName")
                            }
                            PCheckBox { // ShowPosition
                                id: showPositionCheckBox
                                text: "Show Position"
                                checked: zoo.getBool("debug", "ShowPosition")
                            }
                            PCheckBox { // ShowAnimalAIInfo
                                id: showAnimalAIInfoCheckBox
                                text: "Show Animal AI Info"
                                checked: zoo.getBool("debug", "ShowAnimalAIInfo")
                            }
                            PCheckBox { // ShowGuesAIInfo
                                id: showGuestAIInfoCheckBox
                                text: "Show Guest AI Info"
                                checked: zoo.getBool("debug", "ShowGuestAIInfo")
                            }
                            PCheckBox { // ShowStaffAIInfo
                                id: showStaffAIInfoCheckBox
                                text: "Show Staff AI Info"
                                checked: zoo.getBool("debug", "ShowStaffAIInfo")
                            }
                            PCheckBox { // ShowStatusVars
                                id: showStatusVarsCheckBox
                                text: "Show Status Variables"
                                checked: zoo.getBool("debug", "ShowStatusVars")
                            }
                            PCheckBox { // ShowFunctionCall
                                id: showFunctionCallCheckBox
                                text: "Show Function Calls"
                                checked: zoo.getBool("debug", "ShowFunctionCall")
                            }
                            PCheckBox { // ShowEvents
                                id: showEventsCheckBox
                                text: "Show Events"
                                checked: zoo.getBool("debug", "ShowEvents")
                            }
                            PCheckBox { // ShowBuildingAIInfo
                                id: showBuildingAIInfoCheckBox
                                text: "Show Building AI Info"
                                checked: zoo.getBool("debug", "ShowBuildingAIInfo")
                            }
                            PCheckBox { // ShowSelected
                                id: showSelectedCheckBox
                                text: "Show Selected"
                                checked: zoo.getBool("debug", "ShowSelected")
                            }
                            PCheckBox { // ShowFrame
                                id: showFrameCheckBox
                                text: "Show Frame"
                                checked: zoo.getBool("debug", "ShowFrame")
                            }
                            PCheckBox { // ShowGoal
                                id: showGoalCheckBox
                                text: "Show Goal"
                                checked: zoo.getBool("debug", "ShowGoal")
                            }
                        }
                        ControlPanel {
                            id: advancedDebugControls
                            label: "Advanced Settings"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false
                            PCheckBox { // sendLogFile
                                id: sendLogFileCheckBox
                                text: "Send Log File"
                                checked: zoo.getBool("debug", "sendLogFile")
                            }
                            PCheckBox { // sendDebugger
                                id: sendDebuggerCheckBox
                                text: "Send Debugger"
                                checked: zoo.getBool("debug", "sendDebugger")
                            }
                            PTextField { // logCutOff
                                id: logCutOffField
                                title: "Log Cut-off"
                                Layout.fillWidth: true
                                text: zoo.getString("debug", "logCutoff")
                            }
                            PCheckBox { // deltaLog1
                                id: deltaLog1CheckBox
                                text: "Delta Log 1"
                                checked: zoo.getBool("debug", "deltaLog1")
                            }
                            PCheckBox { // deltaLog0
                                id: deltaLog0CheckBox
                                text: "Delta Log 0"
                                checked: zoo.getBool("debug", "deltaLog0")
                            }
                        }
                    }
                }


            }

        }

    }
}
