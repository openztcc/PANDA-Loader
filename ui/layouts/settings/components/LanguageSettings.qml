import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import PandaUI 1.0

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
            text: zoo.getString(modelData.section, modelData.key)
        }
    }
}
