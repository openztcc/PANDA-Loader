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
        spacing: 10

        Row {
            id: switchRow
            width: parent.width
            height: 30
            visible: controlGroup.showSwitch || controlGroup.label !== ""
            spacing: 5

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
                anchors.left: parent.left
            }
        
            Label {
                id: titleLabel
                text: controlGroup.label
                font.pixelSize: 12
                color: controlGroup.enabled ? "#E8E8CF" : "#A0A0A0"
                anchors.left: {
                    if (controlGroup.showSwitch) {
                        return controlSwitch.right
                    } else {
                        return parent.left
                    }
                }
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
            anchors.top: {
                if (switchRow.visible) {
                    return switchRow.bottom
                } else {
                    return parent.top
                }
            }

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            background: Rectangle {
                id: bg
                anchors.fill: parent
                color: "#627D58"
                radius: 5
                border.width: 1
                border.color: Qt.darker(color, 1.2)
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
