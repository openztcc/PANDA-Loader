import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

pragma ComponentBehavior: Bound

Pane {
    id: mainContent
    property var mainColor: "#2c6431"
    height: parent.height
    Material.background: "#9daa9e"
    anchors.fill: parent
    padding: 0
    spacing: 0

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: settingsPane
            Layout.fillHeight: true
            Layout.preferredWidth: 200

            color: mainContent.mainColor

            Column {
                anchors.fill: parent
                spacing: 0

                SettingsButton {
                    text: "PANDA Settings"
                    source: "qrc:/icons/paw.svg"
                    color: mainContent.mainColor
                }

                SettingsButton {
                    text: "Video"
                    source: "qrc:/icons/tv.svg"
                    color: mainContent.mainColor
                }

                SettingsButton {
                    text: "Sound"
                    source: "qrc:/icons/sound.svg"
                    color: mainContent.mainColor
                }

                SettingsButton {
                    text: "Gameplay"
                    source: "qrc:/icons/keyboard.svg"
                    color: mainContent.mainColor
                }

                SettingsButton {
                    text: "Scenarios"
                    source: "qrc:/icons/article.svg"
                    color: mainContent.mainColor
                }

                SettingsButton {
                    text: "Resource Paths"
                    source: "qrc:/icons/folder.svg"
                    color: mainContent.mainColor
                }

                SettingsButton {
                    text: "Advanced"
                    source: "qrc:/icons/plumbing.svg"
                    color: mainContent.mainColor
                }
            }

        }

        Rectangle {
            id: settingsPaneDivider
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 1
            color: "#4d4d4d"
        }

        Pane {
            id: settingsRightPane
            Layout.preferredWidth: parent.width - settingsPane.width - settingsPaneDivider.width
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
            background: {
                color: "#9daa9e"
            }
            opacity: 0.8

            padding: 10

            ColumnLayout {
                id: settingsContent
                anchors.fill: parent
                spacing: 4

                StackView {
                    id: settingsStack
                    anchors.fill: parent
                    initialItem: pandaSettings
                }

                Component {
                    id: pandaSettings
                    // Forms and fields
                    SettingsPane {
                        id: pandaSettingsPane
                        TextField {
                            id: pandaSettingsField
                            placeholderText: "PANDA Settings"
                            color: "#ffffff"
                            Layout.fillWidth: true
                        }
                    }

                }

                Component {
                    id: videoSettings
                    SettingsPane {
                        id: videoSettingsPane
                        TextField {
                            id: videoSettingsField
                            placeholderText: "Video Settings"
                            color: settingsPane.color
                            Layout.fillWidth: true
                        }
                    }
                }

                Component {
                    id: soundSettings
                    SettingsPane {
                        id: soundSettingsPane
                        TextField {
                            id: soundSettingsField
                            placeholderText: "Sound Settings"
                            color: settingsPane.color
                            Layout.fillWidth: true
                        }
                    }
                }

                Component {
                    id: gameplaySettings
                    SettingsPane {
                        id: gameplaySettingsPane
                        TextField {
                            id: gameplaySettingsField
                            placeholderText: "Gameplay Settings"
                            color: settingsPane.color
                            Layout.fillWidth: true
                        }
                    }
                }

                Component {
                    id: scenariosSettings
                    SettingsPane {
                        id: scenariosSettingsPane
                        TextField {
                            id: scenariosSettingsField
                            placeholderText: "Scenarios Settings"
                            color: settingsPane.color
                            Layout.fillWidth: true
                        }
                    }
                }

                Component {
                    id: resourcePathsSettings
                    SettingsPane {
                        id: resourcePathsSettingsPane
                        TextField {
                            id: resourcePathsSettingsField
                            placeholderText: "Resource Paths Settings"
                            color: settingsPane.color
                            Layout.fillWidth: true
                        }
                    }
                }

                Component {
                    id: advancedSettings
                    SettingsPane {
                        id: advancedSettingsPane
                        TextField {
                            id: advancedSettingsField
                            placeholderText: "Advanced Settings"
                            color: settingsPane.color
                            Layout.fillWidth: true
                        }
                    }
                }

            }
        
        }

    }
}
