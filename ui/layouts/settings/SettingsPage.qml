import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects
import PandaUI 1.0

pragma ComponentBehavior: Bound

LayoutFrame {
    id: mainContent
    property var mainColor: "#2c6431"
    property var mainTextColor: "#E8E8CF"
    property var rightPaneColor: "#57704E"
    Layout.fillWidth: true
    Layout.fillHeight: true

    property var dirtyLaundry: []
    property var currentPage: null
    property var modelData: null
    property var currentPane: null

    function replaceSettingsPane(pane, stack, currentButton) {
        settingsStack.replace(pane)
        if (settingsPane.currentButton && settingsPane.currentButton !== currentButton) {
            settingsPane.currentButton.current = false
        }
        currentButton.current = true
        settingsPane.currentButton = currentButton
    }

    SimpleModal {
        id: confirmChangesModal
        title: "Unsaved Changes"
        // showCancel: true
        message: "You have unsaved changes. Do you want to discard them?"
        // onSaved: {
        //     console.log("zoo.ini changes saved")
        //     zoo.saveConfig()
        //     mainContent.cancelledNavigation = false
        //     replaceSettingsPane(mainContent.currentPane, settingsStack, mainContent.currentPage)
        //     close()
        // }
        // onDiscarded: {
        //     zoo.revertChanges()
        //     console.log("zoo.ini changes reverted")
        //     mainContent.cancelledNavigation = false
        //     replaceSettingsPane(mainContent.currentPane, settingsStack, mainContent.currentPage)
        //     close()
        // }

        // onCancelled: {
        //     console.log("Cancelled navigation from current settings pane")
        //     mainContent.cancelledNavigation = true
        //     close()
        // }
    }

    RowLayout {
        id: mainLayout
        Layout.fillWidth: true
        Layout.fillHeight: true
        spacing: 0

        Rectangle {
            id: settingsPane
            Layout.fillHeight: true
            Layout.preferredWidth: 200
            Layout.alignment: Qt.AlignLeft

            color: mainContent.mainColor

            property var currentButton: settingsButtonsRepeater.itemAt(0)

            Rectangle {
                z: 1
                color: Qt.darker(mainContent.rightPaneColor, 1.2)
                width: 1
                height: parent.height
                anchors.right: parent.right
            }

            Column {
                anchors.fill: parent
                spacing: 0

                // start button highlighted at panda settings
                Component.onCompleted: {
                    replaceSettingsPane(pandaSettings, settingsStack, settingsButtonsRepeater.itemAt(0))
                }
                // Settings Buttons
                Repeater {
                    id: settingsButtonsRepeater
                    model: [
                        {text: "PANDA Settings", icon: "qrc:/icons/paw.svg", pane: pandaSettings},
                        {text: "Video", icon: "qrc:/icons/tv.svg", pane: videoSettings},
                        {text: "Sound", icon: "qrc:/icons/sound.svg", pane: soundSettings},
                        {text: "Gameplay", icon: "qrc:/icons/keyboard.svg", pane: gameplaySettings},
                        {text: "UI", icon: "qrc:/icons/ui.svg", pane: uiSettings},
                        {text: "Scenarios", icon: "qrc:/icons/article.svg", pane: scenariosSettings},
                        {text: "Resource Paths", icon: "qrc:/icons/folder.svg", pane: resourcePathsSettings},
                        {text: "Language", icon: "qrc:/icons/language.svg", pane: languageSettings},
                        {text: "Debug Settings", icon: "qrc:/icons/plumbing.svg", pane: debugSettings}
                    ]
                    delegate: SettingsButton {
                        required property var modelData
                        required property int index
                        text: modelData.text
                        source: modelData.icon
                        color: mainContent.mainColor
                        fg: mainContent.mainTextColor

                        Component.onCompleted: {
                            mainContent.modelData = modelData
                        }

                        onClicked: {
                            let currentPageBackup = mainContent.currentPage
                            let currentPaneBackup = modelData.pane
                            mainContent.currentPage = settingsButtonsRepeater.itemAt(index)
                            mainContent.currentPane = modelData.pane

                            // if (zoo.dirty) {
                            //     confirmChangesModal.open()
                            // } else {
                            //     mainContent.cancelledNavigation = false
                            // }

                            // if (!mainContent.cancelledNavigation) {
                            //     replaceSettingsPane(modelData.pane, settingsStack, mainContent.currentPage)
                            // }
                            
                            // mainContent.cancelledNavigation = true
                            if (zoo.dirty) {
                                confirmChangesModal.ask().then(function(result) {
                                    if (result === "save") {
                                        console.log("User chose Save")
                                        zoo.saveConfig()
                                        replaceSettingsPane(modelData.pane, settingsStack, mainContent.currentPage)
                                    } else if (result === "discard") {
                                        console.log("User chose Discard")
                                        zoo.revertChanges()
                                        replaceSettingsPane(modelData.pane, settingsStack, mainContent.currentPage)
                                    } else {
                                        console.log("User cancelled")
                                        mainContent.currentPage = currentPageBackup
                                        mainContent.currentPane = currentPaneBackup
                                    }
                                })
                            } else {
                                replaceSettingsPane(modelData.pane, settingsStack, mainContent.currentPage)
                            }
                        }                         
                    }
                }
            }
        }

        Pane {
            id: settingsRightPane
            Layout.preferredWidth: parent.width - settingsPane.width
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
            Layout.fillWidth: true
            Material.background: mainContent.rightPaneColor

            ColumnLayout {
                id: settingsContent
                anchors.fill: parent
                anchors.topMargin: 10
                spacing: 4

                StackView {
                    id: settingsStack
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    Layout.fillWidth: true
                    initialItem: pandaSettings

                    Component.onCompleted: {
                        mainContent.replaceSettingsPane(pandaSettings, settingsStack, settingsButtonsRepeater.itemAt(1))
                    }
                }

                Component {
                    id: pandaSettings
                    PandaSettings {}
                }

                // videoSettings
                Component {
                    id: videoSettings
                    VideoSettings {}
                }

                // soundSettings
                Component {
                    id: soundSettings
                    SoundSettings {}
                }

                // gameplaySettings
                Component {
                    id: gameplaySettings
                    GameplaySettings {}
                }

                // uiSettings
                Component {
                    id: uiSettings
                    UISettings {}
                }

                // scenariosSettings
                Component {
                    id: scenariosSettings
                    ScenarioSettings {}
                }

                // resourcePathsSettings
                Component {
                    id: resourcePathsSettings
                    ResourcePathsSettings {}
                }

                // languageSettings
                Component {
                    id: languageSettings
                    LanguageSettings {}
                }

                // debugSettings
                Component {
                    id: debugSettings
                    DebugSettings {}
                }

                // top bar with save changes button
                SettingsConfirmationBar{
                    id: settingsConfirmationBar

                    onDiscarded: () => {
                        replaceSettingsPane(mainContent.currentPane, settingsStack, mainContent.currentPage)
                    }
                }
            }

            Connections {
                target: zoo
                onDirtyChanged: (dirty) => {
                    if (dirty) {
                        settingsConfirmationBar.visible = true
                    } else {
                        settingsConfirmationBar.visible = false
                    }
                }
            }

        }

    }
}
