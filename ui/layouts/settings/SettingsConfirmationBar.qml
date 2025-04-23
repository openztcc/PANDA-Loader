    import QtQuick
    import QtQuick.Controls
    import QtQuick.Layouts
    import PandaUI 1.0

    Rectangle {
        id: confirmChangesBar
        property bool hovered: false
        property var config: null
        color: Qt.darker("#57704E", 1.2)
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 60
        Layout.fillHeight: false
        radius: 5
        z: 20
        opacity: hovered ? 1.0 : 0.8
        Behavior on opacity { NumberAnimation { duration: 150 } }
        visible: true
        Behavior on visible { NumberAnimation { duration: 150 } }

        signal discarded()

        HoverHandler {
            id: hoverParent
            target: null
            onHoveredChanged: parent.hovered = hoverParent.hovered
        }

        RowLayout {
            anchors.fill: parent
            spacing: 5

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: saveChangesButton
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.preferredWidth: 150
                flat: true
                background: Rectangle {
                    color: saveChangesButton.hovered ? Qt.darker("#57704E", 1.4) : "#57704E"
                    radius: 5
                    border.color: Qt.darker("#FED286", 1.4)
                    border.width: 2

                    HoverHandler {
                        id: hoverHandle
                        cursorShape: Qt.PointingHandCursor
                        // onHoveredChanged: {
                        //     confirmChangesBar.hovered = saveChangesButton.hovered
                        // }
                    }
                }
                contentItem: Text {
                    id: saveChangesText
                    text: "Save Changes"
                    color: "#FED286"
                    font.pixelSize: 12
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    if (state.dirty) {
                        config.saveConfig()
                        console.log("config.ini changes saved")
                    } else {
                        console.log("No changes to save")
                    }
                }
            }
            Button {
                id: discardChangesButton
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                flat: true
                Layout.preferredWidth: 150

                background: Rectangle {
                    color: hoverHandle2.hovered ? Qt.darker(mainContent.mainColor, 1.2) : Qt.darker(mainContent.mainColor, 1.3)
                    radius: 5
                    border.color: Qt.darker(mainContent.mainColor, 1.7)
                    border.width: 2

                    HoverHandler {
                        id: hoverHandle2
                        cursorShape: Qt.PointingHandCursor
                    }
                }

                contentItem: Text {
                    id: discardChangesText
                    text: "Discard Changes"
                    color: "#ffffff"
                    font.pixelSize: 12
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    if (state.dirty) {
                        config.revertChanges()
                        confirmChangesBar.discarded()
                        console.log("Reverted changes to config.ini")
                    } else {
                        console.log("No changes to save")
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
