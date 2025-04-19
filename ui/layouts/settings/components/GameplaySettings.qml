import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

SettingsPane {
    id: gameplaySettingsPane

    signal dataChanged(var section, var key, var value)

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.updateTable(section, key, value) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))
    }

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

            onTextChange: (data) => {
                gameplaySettingsPane.dataChanged(modelData.section, modelData.key, data)
            }
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

                onTextChange: (data) => {
                    gameplaySettingsPane.dataChanged(modelData.section, modelData.key, data)
                }
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
                {section: "UI", key: "keyScrollX", label: "Key Scroll X"},
                {section: "UI", key: "keyScrollY", label: "Key Scroll Y"},
            ]
            delegate: PTextField {
                required property var modelData
                title: modelData.label
                Layout.fillWidth: true
                descriptionText: "Rate of panning in the x or y direction"
                text: zoo.getString("UI", modelData.key)

                onTextChange: (data) => {
                    gameplaySettingsPane.dataChanged(modelData.section, modelData.key, data)
                }
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
                {section: "Map", key: "mapX", label: "Map X"},
                {section: "Map", key: "mapY", label: "Map Y"}
            ]
            delegate: PTextField {
                required property var modelData
                title: modelData.label
                Layout.fillWidth: true
                descriptionText: "Size of the map"
                text: zoo.getString("Map", modelData.key)

                onTextChange: (data) => {
                    gameplaySettingsPane.dataChanged(modelData.section, modelData.key, data)
                }
            }
        }
    }
}
