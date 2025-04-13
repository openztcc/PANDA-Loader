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

    ColumnLayout {
        id: controlColumn
        anchors.fill: parent
        spacing: 10
        width: parent.width

        Row {
            id: switchRow
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            visible: controlGroup.showSwitch || controlGroup.label !== ""
            spacing: 5
            Layout.bottomMargin: 5

            PCheckBox {
                id: controlSwitch
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
                    controlGroup.enabled = controlSwitch.checked
                    if (controlGroup.data) {
                        controlGroup.data.enabled = controlSwitch.checked
                    }
                }
                visible: controlGroup.showSwitch
                anchors.left: parent.left
                implicitHeight: 30
            }
        
            Label {
                id: titleLabel
                text: controlGroup.label
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 12
                color: controlGroup.enabled ? "#E8E8CF" : "#A0A0A0"
                anchors.left: {
                    if (controlGroup.showSwitch) {
                        return controlSwitch.right
                    } else {
                        return parent.left
                    }
                }
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
            Layout.fillWidth: true
            enabled: controlGroup.enabled
            Layout.preferredHeight: implicitHeight
            Layout.bottomMargin: 10
            opacity: controlGroup.enabled ? 1 : 0.5

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
                width: parent.width
                anchors.margins: 5
                spacing: 5
                enabled: controlGroup.enabled
            }
        }

    }

}
