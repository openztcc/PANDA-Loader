import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import PandaUI 1.0

SettingsPane {
    id: scenariosSettingsPane

    signal dataChanged(var section, var key, var value)

    onDataChanged: (section, key, value) => {
        console.log("Data changed:", section, key, value)
        zoo.setValue(key, value, section) // update table sets data dirty, do not block
        console.log("Is data dirty?: " + (zoo.dirty ? "true" : "false"))
    }

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
            checked: zoo.getValue("UI", modelData.key)

            onCheckChanged: (data) => {
                    scenariosSettingsPane.dataChanged(modelData.section, modelData.key, data.toString())
            }

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
                checked: zoo.getValue(modelData.section, modelData.key)

                onCheckChanged: (data) => {
                        scenariosSettingsPane.dataChanged(modelData.section, modelData.key, data.toString())
                }

            }
        }

        // PCheckBox { // tutorial
        //     id: tutorialZooCheckBox
        //     label: "Completed Tutorial Zoo"
        // }
    }
}
