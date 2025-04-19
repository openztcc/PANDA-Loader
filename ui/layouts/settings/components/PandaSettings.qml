import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

// settingsPane
SettingsPane {
    id: pandaSettingsPane

    signal dataChanged(var section, var key, var value)

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.updateTable(section, key, value) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))

    }


    Repeater {
        id: pandaSettingsRepeater
        model: [
            {key: "pandaHomePath", label: "PANDA Home Path", description: "Path to the PANDA home directory", enabled: false, isFileBrowser: false},
            {key: "zooGamePath", label: "Zoo Game Path", description: "Path to the zoo.exe install directory", enabled: true, isFileBrowser: true},
        ]
        delegate: PTextField {
            required property var modelData
            title: modelData.label
            Layout.fillWidth: true
            descriptionText: modelData.description
            isFileBrowser: modelData.isFileBrowser
            enabled: modelData.enabled
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
