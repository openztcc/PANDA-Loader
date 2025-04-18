import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

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
