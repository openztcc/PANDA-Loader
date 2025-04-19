import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import PandaUI 1.0

SettingsPane {
    id: uiSettingsPane

    signal dataChanged(var section, var key, var value)

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.updateTable(section, key, value) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))

    }


    PTextField { // defaultEditCharLimit
        id: defaultEditCharLimitField
        title: "Default Edit Character Limit"
        Layout.fillWidth: true
        descriptionText: "Default character limit for edit fields"
        text: zoo.getString("UI", "defaultEditCharLimit")

        onTextChange: (data) => {
            uiSettingsPane.dataChanged("UI", modelData.key, data)
        }

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

            onTextChange: (data) => {
                uiSettingsPane.dataChanged("UI", modelData.key, data)
            }

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
                {section: "UI", key: "maxShortTooltipWidth", label: "Max Short Tooltip Width"},
                {section: "UI", key: "maxLongTooltipWidth", label: "Max Long Tooltip Width"}
            ]
            delegate: PTextField {
                required property var modelData
                title: modelData.label
                Layout.fillWidth: true
                descriptionText: "Maximum width of tooltips"
                text: zoo.getString("UI", modelData.key)

                onTextChange: (data) => {
                    uiSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

            }
        }
    }

    PTextField { // helpType
        id: helpTypeField
        title: "Help Type"
        Layout.fillWidth: true
        text: zoo.getString("UI", "helpType")

        onTextChange: (data) => {
            uiSettingsPane.dataChanged("UI", modelData.key, data)
        }

    }

    // ----------------------------- MESSAGE DISPLAY SETTINGS
    ControlPanel { // MessageDisplay
        id: messageDisplayControl
        label: "Message Display"
        showSwitch: true
        Layout.fillWidth: true
        checked: zoo.getBool("UI", "MessageDisplay")

        // TODO: update message display

        PTextField { // minimumMessageInterval
            id: minimumMessageIntervalField
            title: "Minimum Message Interval"
            Layout.fillWidth: true
            descriptionText: "Minimum interval between messages"
            text: zoo.getString("UI", "minimumMessageInterval")

            onTextChange: (data) => {
                uiSettingsPane.dataChanged("UI", modelData.key, data)
            }

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

                onTextChange: (data) => {
                    uiSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

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
                onTextChange: (data) => {
                    uiSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

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

                onTextChange: (data) => {
                    uiSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

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

                onTextChange: (data) => {
                    uiSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

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

                onTextChange: (data) => {
                    uiSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

            }
        }
    }                        

}
