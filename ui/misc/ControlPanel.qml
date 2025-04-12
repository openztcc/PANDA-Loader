import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: controlGroup
    property var data: null
    property bool enabled: true
    property bool hasFrame: true
    property var label: ""
    property bool showSwitch: false
    default property alias contents: panelContents.children
    width: parent.width

    Column {
        id: controlColumn
        anchors.fill: parent

        Row {
            id: switchRow

            CheckBox {
                id: controlSwitch
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
                    controlGroup.enabled = controlSwitch.checked
                    if (controlGroup.data) {
                        controlGroup.data.enabled = controlSwitch.checked
                    }
                }
                visible: controlGroup.showSwitch
                enabled: controlGroup.enabled
            }
        
            Label {
                id: titleLabel
                text: controlGroup.label
                font.pixelSize: 12
                color: controlGroup.enabled ? "#E8E8CF" : "#A0A0A0"
                anchors.left: parent.left
                anchors.leftMargin: 10
                visible: {
                    if (controlGroup.label !== "") {
                        return true
                    } else {
                        return false
                    }
                }
            }
        }

        Pane {
            id: controlPanel
            anchors.fill: parent

            background: Rectangle {
                id: bg
                anchors.fill: parent
                color: "#627D58"
                radius: 5
                border.width: 1
                border.color: Qt.darker(color, 1.2)
                border.radius: 5
                opacity: 0.9
                visible: controlGroup.hasFrame
            }

            ColumnLayout {
                id: panelContents
                anchors.fill: parent
                anchors.margins: 5
                spacing: 5
                enabled: controlGroup.enabled
            }
        }

    }

}