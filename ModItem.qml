import QtQuick

Item {
    property string text: "No text"
    anchors.fill: parent

    Pane {
        anchors.fill: parent
        Material.background:  modArea.containsPress ? Qt.lighter("#f7fbf2", 0.8) : modArea.containsMouse ? Qt.darker("#f7fbf2", 0.01) : "#f7fbf2"
        padding: 12
        anchors.bottomMargin: 1

        signal clicked()

        contentItem: RowLayout {
            id: modMeta
            spacing: 12
            Rectangle {
                id: modImg
                width: 44
                height: 30
                color: "#BCD0C3"
            }

            ColumnLayout {

                Layout.fillWidth: true
                spacing: 3

                // category
                Label {
                    text: "Buildings"
                    font.pixelSize: 10
                }

                // name of mod
                Label {
                    text: model.modTitle
                    font.pixelSize: 12
                    color: "#424940"
                }

                // author(s)
                Label {
                    text: "by " + model.modAuthor
                    font.pixelSize: 10
                }
            }

            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                id: modCheck
                z: 1
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: -15
                Layout.preferredWidth: 20
                checked: true
                Material.accent: "#376a3e"
                enabled: true
                onCheckedChanged: {
                    console.log("Checkbox changed:", model.modTitle, checked)
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    event.accepted = true;
                }
                propagateComposedEvents: false
            }
        }
        MouseArea {
            id: modArea
            anchors.fill: modMeta
            cursorShape: Qt.PointingHandCursor
            onClicked: {
                modController.selectMod(index);
                console.log("Mod clicked:", model.modTitle);

                modDetailsText.text = model.modDescription;
            }
            hoverEnabled: true
        }

    }

}

