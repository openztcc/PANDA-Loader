    import QtQuick
    import QtQuick.Controls
    import QtQuick.Layouts
    import PandaUI 1.0
    
    Rectangle {
        id: confirmChangesBar
        property bool hovered: false
        color: Qt.darker("#57704E", 1.2)
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        Layout.preferredHeight: 60
        Layout.fillHeight: false
        radius: 5
        z: 20
        opacity: hovered ? 1.0 : 0.8
        visible: zoo.dirty ? true : false

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
                text: "Save Changes"
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
                onClicked: {
                    if (zoo.dirty) {
                        zoo.saveConfig()
                    } else {
                        console.log("No changes to save")
                    }
                }
            }
            Button {
                id: discardChangesButton
                text: "Discard"
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                flat: true
                Layout.preferredWidth: 150

                background: Rectangle {
                    color: hoverHandle2.hovered ? Qt.darker(mainContent.mainColor, 1.2) : mainContent.mainColor
                    radius: 5

                    HoverHandler {
                        id: hoverHandle2
                        cursorShape: Qt.PointingHandCursor
                    }
                }
                onClicked: {
                    if (zoo.dirty) {
                        zoo.saveConfig()
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
