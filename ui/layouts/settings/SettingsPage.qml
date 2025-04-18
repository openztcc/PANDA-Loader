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

    property int dirtyTracker: 0

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

            property var currentButton: settingsButtonsRepeater.itemAt(0)

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

                // start button highlighted at panda settings
                Component.onCompleted: {
                    replaceSettingsPane(pandaSettings, settingsStack, settingsButtonsRepeater.itemAt(0))
                }
                // Settings Buttons
                Repeater {
                    id: settingsButtonsRepeater
                    model: [
                        {text: "PANDA Settings", icon: "qrc:/icons/paw.svg", pane: pandaSettings},
                        {text: "Video", icon: "qrc:/icons/tv.svg", pane: videoSettings},
                        {text: "Sound", icon: "qrc:/icons/sound.svg", pane: soundSettings},
                        {text: "Gameplay", icon: "qrc:/icons/keyboard.svg", pane: gameplaySettings},
                        {text: "UI", icon: "qrc:/icons/ui.svg", pane: uiSettings},
                        {text: "Scenarios", icon: "qrc:/icons/article.svg", pane: scenariosSettings},
                        {text: "Resource Paths", icon: "qrc:/icons/folder.svg", pane: resourcePathsSettings},
                        {text: "Language", icon: "qrc:/icons/language.svg", pane: languageSettings},
                        {text: "Debug Settings", icon: "qrc:/icons/plumbing.svg", pane: debugSettings}
                    ]
                    delegate: SettingsButton {
                        required property var modelData
                        required property int index
                        text: modelData.text
                        source: modelData.icon
                        color: mainContent.mainColor
                        fg: mainContent.mainTextColor

                        onClicked: {
                            var identifier = settingsButtonsRepeater.itemAt(index)
                            if (!zoo.dirty) {
                                replaceSettingsPane(modelData.pane, settingsStack, identifier)
                            } else {
                                var result = Qt.confirm("Unsaved Changes", "You have unsaved changes. Do you want to discard them?")
                                if (result === Qt.Yes) {
                                    zoo.dirty = false
                                    replaceSettingsPane(modelData.pane, settingsStack, identifier)
                                } else if (result === Qt.No) {
                                    return
                                } else if (result === Qt.Cancel) {
                                    return
                                } else {
                                    zoo.dirty = false
                                    replaceSettingsPane(modelData.pane, settingsStack, identifier)
                                }
                            }
                        } 
                        
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
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    initialItem: pandaSettings

                    Component.onCompleted: {
                        mainContent.replaceSettingsPane(pandaSettings, settingsStack, settingsButtonsRepeater.itemAt(1))
                    }
                }

                // settingsPane
                Component {
                    id: pandaSettings
                    // Forms and fields
                    SettingsPane {
                        id: pandaSettingsPane

                        Repeater {
                            id: pandaSettingsRepeater
                            model: [
                                {key: "pandaHomePath", label: "PANDA Home Path", description: "Path to the PANDA home directory", enabled: false},
                                {key: "zooGamePath", label: "Zoo Game Path", description: "Path to the zoo.exe install directory", enabled: true},
                            ]
                            delegate: PTextField {
                                required property var modelData
                                title: modelData.label
                                Layout.fillWidth: true
                                descriptionText: modelData.description
                                isFileBrowser: modelData.enabled
                                enabled: false
                                text: psettings[modelData.key]()
                            }
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

                // videoSettings
                Component {
                    id: videoSettings
                    SettingsPane {
                        id: videoSettingsPane
                        signal dataChanged(var section, var key, var value)
                        // Group of settings for drawfps, drawfpsx, drawfpsy

                        onDataChanged: (section, key, value) => {
                            zoo.updateTable(section, key, value)
                        }

                        Repeater {
                            id: graphicsSettingsRepeater
                            model: [
                                {key: "loadHalfAnims", section: "advanced", label: "Load Half Animations", description: "Load half animations"},
                            ]
                            delegate: PCheckBox {
                                required property var modelData
                                text: modelData.label
                                checked: zoo.getBool(modelData.section, modelData.key)

                                onCheckChanged: (data) => {
                                        videoSettingsPane.dataChanged(modelData.section, modelData.key, data)
                                }
                            }
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

                                onCheckChanged: (data) => {
                                    videoSettingsPane.dataChanged("user", "fullscreen", data)
                                }
                            }

                            Repeater {
                                id: displaySettingsRepeater
                                model: [
                                    {key: "screenwidth", section: "user", label: "Screen Width", description: "Width of the screen resolution"},
                                    {key: "screenheight", section: "user", label: "Screen Height", description: "Height of the screen resolution"},
                                    {key: "UpdateRate", section: "user", label: "Update Rate", description: "Update rate of the game"},
                                    {key: "DrawRate", section: "user", label: "Draw Rate", description: "Draw rate of the game"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: modelData.description
                                    text: zoo.getString(modelData.section, modelData.key)

                                    onTextChange: (data) => {
                                        videoSettingsPane.dataChanged(modelData.section, modelData.key, data)
                                    }
                                }
                            }
                        }

                        ControlPanel { // drawfps
                            id: drawFPSControl
                            label: "Display FPS Counter"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: zoo.getBool("debug", "drawfps")

                            onControlGroupChecked: (data) => {
                                videoSettingsPane.dataChanged("debug", "drawfps", data)
                            }

                            contents: Repeater {
                                id: drawFPSControlRepeater
                                model: [
                                    {key: "drawfpsx", section: "debug", label: "X Position", description: "X position of the FPS counter"},
                                    {key: "drawfpsy", section: "debug", label: "Y Position", description: "Y position of the FPS counter"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: modelData.description
                                    text: zoo.getString(modelData.section, modelData.key)

                                    onTextChange: (data) => {
                                        videoSettingsPane.dataChanged(modelData.section, modelData.key, data)
                                    }
                                }
                            }
                        }
                    }
                }

                // soundSettings
                Component {
                    id: soundSettings
                    SettingsPane {
                        id: soundSettingsPane
                        PCheckBox { // use8BitSound
                            id: use8BitSoundCheckBox
                            text: "Use 8 Bit Sound"
                            checked: zoo.getBool("advanced", "use8BitSound")
                        }

                        Repeater {
                            id: soundSettingsRepeater
                            model: [
                                {key: "userAttenuation", section: "UI", label: "User Attenuation", description: "Attenuation of the user"},
                                {key: "completedExhibitAttenuation", section: "UI", label: "Completed Exhibit Attenuation", description: "Attenuation of the completed exhibit"},
                            ]
                            delegate: PTextField {
                                required property var modelData
                                title: modelData.label
                                Layout.fillWidth: true
                                descriptionText: modelData.description
                                text: zoo.getString(modelData.section, modelData.key)
                            }
                        }
                        ControlPanel { // menu music controls
                            id: menuMusicControls
                            label: "Menu Music Enabled"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false

                            contents: Repeater {
                                id: menuMusicControlsRepeater
                                model: [
                                    {key: "menuMusicAttenuation", section: "UI", label: "Menu Music Volume", description: "Volume of the menu music"},
                                    {key: "menuMusic", section: "UI", label: "Menu Music", description: "Path to the menu music file"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: modelData.description
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }
                        ControlPanel { // boot music controls
                            id: bootMusicControls
                            label: "Boot Music Volume"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: bootMusicControlsRepeater
                                model: [
                                    {key: "movievolume1", section: "UI", label: "Volume for Splash Screen 1", description: "Volume for the first splash screen that plays"},
                                    {key: "movievolume2", section: "UI", label: "Volume for Splash Screen 2", description: "Volume for the second splash screen that plays"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: modelData.description
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }
                    }
                }

                // gameplaySettings
                Component {
                    id: gameplaySettings
                    SettingsPane {
                        id: gameplaySettingsPane

                        Repeater {
                            id: gameplaySettingsRepeater
                            model: [
                                {key: "MSStartingCash", section: "UI", label: "Starting Cash", description: "Starting cash in freeform mode"},
                                {key: "MSCashIncrement", section: "UI", label: "Cash Increment", description: "Cash increment in freeform mode"},
                                {key: "MSMinCash", section: "UI", label: "Minimum Cash", description: "Minimum cash in freeform mode"},
                                {key: "MSMaxCash", section: "UI", label: "Maximum Cash", description: "Maximum cash in freeform mode"},                                
                            ]
                            delegate: PTextField {
                                required property var modelData
                                title: modelData.label
                                Layout.fillWidth: true
                                descriptionText: modelData.description
                                text: zoo.getString(modelData.section, modelData.key)
                            }
                        }
                        ControlPanel { // mouse controls 
                            id: controlsSettings
                            label: "Mouse Settings"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: controlsSettingsRepeater
                                model: [
                                    {key: "drag", section: "advanced", label: "Drag", description: "Drag speed"},
                                    {key: "click", section: "advanced", label: "Click", description: "Click speed"},
                                    {key: "normal", section: "advanced", label: "Normal", description: "Normal speed"},
                                    {key: "level", section: "advanced", label: "Level", description: "Level speed"},
                                    {key: "mouseScrollThreshold", section: "UI", label: "Mouse Scroll Threshold", description: "Threshold for mouse scroll"},
                                    {key: "mouseScrollDelay", section: "UI", label: "Mouse Scroll Delay", description: "Delay for mouse scroll"},
                                    {key: "mouseScrollX", section: "UI", label: "Mouse Scroll X", description: "X rate of mouse scroll"},
                                    {key: "mouseScrollY", section: "UI", label: "Mouse Scroll Y", description: "Y rate of mouse scroll"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: modelData.description
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }

                        // ----------------------------- KEYBOARD SETTINGS
                        ControlPanel { // key controls 
                            id: keyControlsSettings
                            label: "Keyboard Settings"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false

                            contents: Repeater {
                                id: keyControlsSettingsRepeater
                                model: [
                                    {key: "keyScrollX", label: "Key Scroll X"},
                                    {key: "keyScrollY", label: "Key Scroll Y"},
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Rate of panning in the x or y direction"
                                    text: zoo.getString("UI", modelData.key)
                                }
                            }
                        }

                        // ----------------------------- MAP SIZE SETTINGS
                        ControlPanel { // map size
                            id: mapSizeControls
                            label: "Map Size"
                            showSwitch: true
                            Layout.fillWidth: true
                            checked: false

                            contents: Repeater {
                                id: mapSizeControlsRepeater
                                model: [
                                    {key: "mapX", label: "Map X"},
                                    {key: "mapY", label: "Map Y"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Size of the map"
                                    text: zoo.getString("Map", modelData.key)
                                }
                            }
                        }
                    }
                }

                // uiSettings
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

                        // ----------------------------- TOOLTIP SETTINGS
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

                        // ----------------------------- TOOLTIP WIDTHS
                        ControlPanel { // Tooltip Widths
                            id: tooltipWidthControl
                            label: "Tooltip Widths"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: tooltipWidthControlsRepeater
                                model: [
                                    {key: "maxShortTooltipWidth", label: "Max Short Tooltip Width"},
                                    {key: "maxLongTooltipWidth", label: "Max Long Tooltip Width"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Maximum width of tooltips"
                                    text: zoo.getString("UI", modelData.key)
                                }
                            }
                        }

                        PTextField { // helpType
                            id: helpTypeField
                            title: "Help Type"
                            Layout.fillWidth: true
                            text: zoo.getString("UI", "helpType")
                        }

                        // ----------------------------- MESSAGE DISPLAY SETTINGS
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
                        // ----------------------------- PROGRESS BAR SETTINGS
                        ControlPanel { // progress bar location
                            id: progressBarControls
                            label: "Progress Bar Settings"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: progressBarControlsRepeater
                                model: [
                                    {key: "progressLeft", label: "Left"},
                                    {key: "progressTop", label: "Top"},
                                    {key: "progressBottom", label: "Bottom"},
                                    {key: "progressRight", label: "Right"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Position of the progress bar"
                                    text: zoo.getString("UI", modelData.key)
                                }
                            }
                        }

                        // ----------------------------- PROGRESS BAR COLOR SETTINGS
                        ControlPanel { // progress bar color
                            id: progressBarColorControls
                            label: "Progress Bar Color"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: progressBarColorControlsRepeater
                                model: [
                                    {key: "progressRed", label: "Red"},
                                    {key: "progressGreen", label: "Green"},
                                    {key: "progressBlue", label: "Blue"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Color value of the progress bar"
                                    text: zoo.getString("UI", modelData.key)
                                }
                            }
                        }

                        // ----------------------------- PROGRESS BAR SHADOW SETTINGS
                        ControlPanel { // progress bar shadow offset
                            id: progressBarShadowControls
                            label: "Progress Bar Shadow Offset"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: progressBarShadowControlsRepeater
                                model: [
                                    {key: "progressShadowXOffset", section: "UI", label: "X Offset"},
                                    {key: "progressShadowYOffset", section: "UI", label: "Y Offset"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Offset of the progress bar shadow"
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }

                        // ----------------------------- PROGRESS BAR SHADOW COLOR
                        ControlPanel { // progress bar shadow color
                            id: progressBarShadowColorControls
                            label: "Progress Bar Shadow Color"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: progressBarShadowColorControlsRepeater
                                model: [
                                    {key: "progressShadowRed", section: "UI", label: "Red"},
                                    {key: "progressShadowGreen", section: "UI", label: "Green"},
                                    {key: "progressShadowBlue", section: "UI", label: "Blue"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Color value of the progress bar shadow"
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }
                        // ----------------------------- LAST WINDOW POSITION SETTINGS
                        ControlPanel { // last window position
                            id: lastWindowPositionControls
                            label: "Last Window Position"
                            Layout.fillWidth: true

                            contents: Repeater {
                                id: lastWindowPositionControlsRepeater
                                model: [
                                    {key: "lastWindowX", section: "UI", label: "Last Window X"},
                                    {key: "lastWindowY", section: "UI", label: "Last Window Y"}
                                ]
                                delegate: PTextField {
                                    required property var modelData
                                    title: modelData.label
                                    Layout.fillWidth: true
                                    descriptionText: "Relative to the main monitor, position where the game was last closed"
                                    text: zoo.getString(modelData.section, modelData.key)
                                }
                            }
                        }                        
                    
                    }
                }

                // scenariosSettings
                Component {
                    id: scenariosSettings
                    SettingsPane {
                        id: scenariosSettingsPane
                        Repeater {
                            id: scenariosSettingsRepeater
                            model: [
                                {key: "zooTycoonCheckBox", section: "UI", label: "Started Zoo Tycoon Tutorial"},
                                {key: "dinoDigsCheckBox", section: "UI", label: "Started Dino Digs Tutorial"},
                                {key: "marineManiaCheckBox", section: "UI", label: "Started Marine Mania Tutorial"}
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

                // resourcePathsSettings
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

                // languageSettings
                Component {
                    id: languageSettings
                    SettingsPane {
                        id: languageSettingsPane
                        content: Repeater  {
                            id: languageSettingsRepeater
                            model: [
                                {key: "lang", section: "language", label: "Language"},
                                {key: "sublang", section: "language", label: "Sub Language"}
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

                // debugSettings
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
                // top bar with save changes button

                Rectangle {
                    id: confirmChangesBar
                    property bool hovered: false
                    color: Qt.darker("#57704E", 1.2)
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    Layout.preferredHeight: 60
                    Layout.fillHeight: false
                    radius: 5
                    z: 20
                    opacity: hovered ? 1.0 : 0.5

                    HoverHandler {
                        id: hoverParent
                        target: null
                        onHoveredChanged: parent.hovered = hoverParent.hovered
                    }

                    RowLayout {
                        anchors.fill: parent
                        spacing: 5

                        Item {
                            Layout.fillWidth: true
                        }

                        Button {
                            id: saveChangesButton
                            text: "Save Changes"
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            Layout.preferredWidth: 150
                            flat: true
                            background: Rectangle {
                                color: saveChangesButton.hovered ? Qt.darker("#57704E", 1.4) : "#57704E"
                                radius: 5
                                border.color: Qt.darker("#FED286", 1.4)
                                border.width: 2

                                HoverHandler {
                                    id: hoverHandle
                                    cursorShape: Qt.PointingHandCursor
                                    // onHoveredChanged: {
                                    //     confirmChangesBar.hovered = saveChangesButton.hovered
                                    // }
                                }
                            }
                            onClicked: {
                                if (zoo.dirty) {
                                    zoo.saveConfig()
                                } else {
                                    console.log("No changes to save")
                                }
                            }
                        }
                        Button {
                            id: discardChangesButton
                            text: "Discard"
                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                            flat: true
                            Layout.preferredWidth: 150

                            background: Rectangle {
                                color: hoverHandle2.hovered ? Qt.darker(mainContent.mainColor, 1.2) : mainContent.mainColor
                                radius: 5

                                HoverHandler {
                                    id: hoverHandle2
                                    cursorShape: Qt.PointingHandCursor
                                }
                            }
                            onClicked: {
                                if (zoo.dirty) {
                                    zoo.saveConfig()
                                } else {
                                    console.log("No changes to save")
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }

            }

        }

    }
}
