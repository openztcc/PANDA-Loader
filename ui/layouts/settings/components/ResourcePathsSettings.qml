import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

SettingsPane {
    id: resourcePathsSettingsPane

    signal dataChanged(var section, var key, var value)

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.setValue(key, value, section) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))

    }

    // custom component for resource paths
    PTextField { // lastfile
        id: lastZooPlayedPath
        title: "Last Zoo Played"
        Layout.fillWidth: true
        isFileBrowser: true
        descriptionText: "Path to the last zoo played"
        text: zoo.getValue("user", "lastfile")

        onTextChange: (data) => {
            resourcePathsSettingsPane.dataChanged("user", "lastfile", data)
        }
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
                text: zoo.getValue(modelData.section, modelData.key)

                onTextChange: (data) => {
                    resourcePathsSettingsPane.dataChanged(modelData.section, modelData.key, data)
                }

            }
        }
    }
}
