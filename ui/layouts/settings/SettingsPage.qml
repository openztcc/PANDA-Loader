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

                // ----------------------------- SCENARIO SETTINGS
                Component {
                    id: scenariosSettings
                    SettingsPane {
                        id: scenariosSettingsPane
                        content: Repeater {
                            id: scenariosSettingsRepeater
                            model: [
                                {key: "zooTycoonCheckBox", label: "Started Zoo Tycoon Tutorial"},
                                {key: "dinoDigsCheckBox", label: "Started Dino Digs Tutorial"},
                                {key: "marineManiaCheckBox", label: "Started Marine Mania Tutorial"}
                            ]
                            delegate: PCheckBox {
                                required property var modelData
                                text: modelData.label
                                checked: zoo.getBool("UI", modelData.key)
                            }
                        }
                        // ----------------------------- SCENARIO CHECKBOXES
                        ControlPanel {
                            id: scenarioControls
                            label: "Completed Scenarios (To unlock other scenarios)"
                            Layout.preferredHeight: implicitHeight
                            Layout.fillWidth: true
                            checked: true

                            contents: Repeater {
                                id: scenarioCheckBoxes
                                model: [
                                    {key: "aa", section: "scenario", label: "Completed Tutorial 1 - Game Controls"},
                                    {key: "ab", section: "scenario", label: "Completed Tutorial 2 - Basic Gameplay"},
                                    {key: "ac", section: "scenario", label: "Completed Tutorial 3 - Making Animals Happy"},
                                    {key: "ad", section: "scenario", label: "Completed Dinosaur Digs: Tutorial 1"},
                                    {key: "ae", section: "scenario", label: "Completed Marine Mania: Tutorial 1"},
                                    {key: "af", section: "scenario", label: "Completed Marine Mania: Tutorial 2"},
                                    {key: "ag", section: "scenario", label: "Completed Marine Mania: Tutorial 3"},
                                    {key: "ba", section: "scenario", label: "Completed Small Zoo (Beginner)"},
                                    {key: "bb", section: "scenario", label: "Completed Seaside Zoo (Beginner)"},
                                    {key: "bc", section: "scenario", label: "Completed Forest Zoo (Beginner)"},
                                    {key: "bd", section: "scenario", label: "Completed Holiday Tree Farm (Beginner)"},
                                    {key: "be", section: "scenario", label: "Completed Dinosaur Digs: Ice Age Animal Zoo (Beginner)"},
                                    {key: "bf", section: "scenario", label: "Completed Marine Mania: Orca Show (Beginner)"},
                                    {key: "bg", section: "scenario", label: "Completed Marine Mania: Seasideville Dolphin Park (Beginner)"},
                                    {key: "bh", section: "scenario", label: "Completed Marine Mania: Shark World (Beginner)"},
                                    {key: "bi", section: "scenario", label: "Completed Marine Mania: Surf and Turf Zoo (Beginner)"},
                                    {key: "ca", section: "scenario", label: "Completed Revitalize Burkitsville Zoo (Intermediate)"},
                                    {key: "cb", section: "scenario", label: "Completed Inner City Zoo (Intermediate)"},
                                    {key: "cc", section: "scenario", label: "Completed Saving the Great Cats (Intermediate)"},
                                    {key: "cd", section: "scenario", label: "Completed Endangered Species Zoo (Intermediate)"},
                                    {key: "ce", section: "scenario", label: "Completed Arctic Zoo (Intermediate)"},
                                    {key: "cf", section: "scenario", label: "Completed Beach Resort Zoo (Intermediate)"},
                                    {key: "cg", section: "scenario", label: "Completed Dinosaur Digs: Valley of the Dinosaurs (Intermediate)"},
                                    {key: "ch", section: "scenario", label: "Completed Dinosaur Digs: Jurassic Zoo (Intermediate)"},
                                    {key: "ci", section: "scenario", label: "Completed Marine Mania: Oceans of the Zoo (Intermediate)"},
                                    {key: "cj", section: "scenario", label: "Completed Marine Mania: Save the Marine Animals (Intermediate)"},
                                    {key: "ck", section: "scenario", label: "Completed Marine Mania: Free Admission Zoo (Intermediate)"},
                                    {key: "cl", section: "scenario", label: "Completed Marine Mania: Aquatic Show Park (Intermediate)"},
                                    {key: "cm", section: "scenario", label: "Completed Dinosaur Digs: Carnivore Zoo (Intermediate)"},
                                    {key: "cn", section: "scenario", label: "Completed Southeast Asian Zoo (Intermediate)"},
                                    {key: "da", section: "scenario", label: "Completed Island Zoo (Advanced)"},
                                    {key: "db", section: "scenario", label: "Completed African Savannah Zoo (Advanced)"},
                                    {key: "dc", section: "scenario", label: "Completed Mountain Zoo (Advanced)"},
                                    {key: "dd", section: "scenario", label: "Completed Tropical Rainforest Zoo (Advanced)"},
                                    {key: "de", section: "scenario", label: "Completed Dinosaur Digs: Dinosaur Island Research Lab (Advanced)"},
                                    {key: "df", section: "scenario", label: "Completed Marine Mania: Marine Conservation (Advanced)"},
                                    {key: "dg", section: "scenario", label: "Completed Marine Mania: Save the Zoo (Advanced)"},
                                    {key: "dh", section: "scenario", label: "Completed Conservation Zoo (Advanced)"},
                                    {key: "ea", section: "scenario", label: "Completed Paradise Island (Very Advanced)"},
                                    {key: "fa", section: "scenario", label: "Completed Breeding Giant Pandas (Very Advanced)"},
                                    {key: "fb", section: "scenario", label: "Completed Dinosaur Digs: Return to Dinosaur Island Research Lab (Very Advanced)"},
                                    {key: "ga", section: "scenario", label: "Completed Dinosaur Digs: Breeding the T.rex (Very Advanced)"},
                                    {key: "gb", section: "scenario", label: "Completed Marine Mania: Giant Marine Park (Very Advanced)"},
                                    {key: "gc", section: "scenario", label: "Completed Marine Mania: Super Zoo (Very Advanced)"},
                                    {key: "gd", section: "scenario", label: "Completed Marine Mania: Ultimate Zoo (Very Advanced)"}                                    
                                ]
                                delegate: PCheckBox {
                                    required property var modelData
                                    text: modelData.label
                                    checked: zoo.getBool(modelData.section, modelData.key)
                                }
                            }

                            // PCheckBox { // tutorial
                            //     id: tutorialZooCheckBox
                            //     label: "Completed Tutorial Zoo"
                            // }
                        }
                    }
                }

                // ----------------------------- RESOURCE PATHS SETTINGS
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
                        // ------------------------------- ADVANCED RESOURCES SETTINGS
                        ControlPanel {
                            id: advancedResControls
                            label: "Advanced Resources"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false

                            contents: Repeater {
                                id: advancedResControlsRepeater
                                model: [
                                    {key: "aimgr", section: "mgr", label: "ZTAIMgr"},
                                    {key: "worldmgr", section: "mgr", label: "ZTWorldMgr"},
                                    {key: "gamemgr", section: "mgr", label: "BFGameMgr"},
                                    {key: "scenariomgr", section: "mgr", label: "ZTScenarioMgr"},
                                    {key: "scriptmgr", section: "mgr", label: "BFScriptMgr"},
                                    {key: "soundmgr", section: "mgr", label: "BFSoundMgr"},
                                    {key: "terrainmgr", section: "mgr", label: "ZTAdvTerrainMgr"},
                                    {key: "res", section: "lib", label: "ZTRes"},
                                    {key: "lang", section: "lib", label: "ZTLang"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }
                    }
                }

                // ----------------------------- LANGUAGE SETTINGS
                Component {
                    id: languageSettings
                    SettingsPane {
                        id: languageSettingsPane
                        content: Repeater  {
                            id: languageSettingsRepeater
                            model: [
                                {key: "lang", label: "Language"},
                                {key: "sublang", label: "Sub Language"}
                            ]
                            delegate: PTextField {
                                required property var modelData
                                title: modelData.label
                                Layout.fillWidth: true
                                text: zoo.getString("language", modelData.key)
                            }
                        }
                    }
                }

                // ----------------------------- DEBUG SETTINGS
                Component {
                    id: debugSettings
                    SettingsPane {
                        id: debugSettingsPane
                        PCheckBox {
                            id: devModeCheckBox
                            text: "Developer Mode Enabled"
                            checked: zoo.getBool("debug", "devModeEnabled")
                        }
                        // ------------------------- DEBUG CONTROLS
                        ControlPanel {
                            id: debugControls
                            label: "Debug Settings (Applies to placed entities)"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: debugControlsRepeater
                                model: [
                                    {key: "ShowAIInfo", section: "debug", label: "Show AI Info"},
                                    {key: "ShowName", section: "debug", label: "Show Name"},
                                    {key: "ShowPosition", section: "debug", label: "Show Position"},
                                    {key: "ShowAnimalAIInfo", section: "debug", label: "Show Animal AI Info"},
                                    {key: "ShowGuestAIInfo", section: "debug", label: "Show Guest AI Info"},
                                    {key: "ShowStaffAIInfo", section: "debug", label: "Show Staff AI Info"},
                                    {key: "ShowStatusVars", section: "debug", label: "Show Status Variables"},
                                    {key: "ShowFunctionCall", section: "debug", label: "Show Function Call"},
                                    {key: "ShowEvents", section: "debug", label: "Show Events"},
                                    {key: "ShowBuildingAIInfo", section: "debug", label: "Show Building AI Info"},
                                    {key: "ShowSelected", section: "debug", label: "Show Selected"},
                                    {key: "ShowFrame", section: "debug", label: "Show Frame"},
                                    {key: "ShowGoal", section: "debug", label: "Show Goal"},
                                ]
                                delegate: PCheckBox {
                                    required property var modelData
                                    text: modelData.label
                                    checked: zoo.getBool(modelData.section, modelData.key)
                                }       
                            }
                        }
                        // ---------------------------- ADVANCED DEBUG SETTINGS
                        ControlPanel {
                            id: advancedDebugControls
                            label: "Advanced Settings"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false
                            
                            contents: Repeater {
                                id: advancedDebugControlsRepeater
                                model: [
                                    {key: "sendLogFile", section: "debug", label: "Send Log File"},
                                    {key: "sendDebugger", section: "debug", label: "Send Debugger"},
                                    {key: "logCutoff", section: "debug", label: "Log Cut-off"},
                                    {key: "deltaLog1", section: "debug", label: "Delta Log 1"},
                                    {key: "deltaLog0", section: "debug", label: "Delta Log 0"}
                                ]
                                delegate: PCheckBox {
                                    required property var modelData
                                    text: modelData.label
                                    checked: zoo.getBool(modelData.section, modelData.key)
                                }
                            }
                        }
                    }
                }


            }

        }

    }
}
