import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

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
