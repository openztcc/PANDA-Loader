import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import PandaUI 1.0

SettingsPane {
    id: languageSettingsPane

    signal dataChanged(var section, var key, var value)

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.updateTable(section, key, value) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))

    }
    content: Repeater  {
        id: languageSettingsRepeater
        model: [
            {key: "lang", section: "language", label: "Language"},
            {key: "sublang", section: "language", label: "Sub Language"}
        ]
        delegate: PTextField {
            required property var modelData
            title: modelData.label
            text: zoo.getString(modelData.section, modelData.key)

            onTextChange: (data) => {
                languageSettingsPane.dataChanged(modelData.section, modelData.key, data)
            }
        }
    }
}
