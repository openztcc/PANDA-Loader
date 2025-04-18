import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

SettingsPane {
    id: videoSettingsPane
    signal dataChanged(var section, var key, var value)
    // Group of settings for drawfps, drawfpsx, drawfpsy

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.updateTable(section, key, value) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))

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
                    videoSettingsPane.dataChanged(modelData.section, modelData.key, data.toString())
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
                videoSettingsPane.dataChanged("user", "fullscreen", data.toString())
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
            videoSettingsPane.dataChanged("debug", "drawfps", data.toString())
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
