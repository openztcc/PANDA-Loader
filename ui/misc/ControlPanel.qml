import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: controlGroup
    property var data: null
    property bool checked: true
    property bool enabled: true
    property bool hasFrame: true
    property var label: ""
    property bool showSwitch: false
    default property alias contents: panelContents.children
    implicitHeight: controlColumn.implicitHeight
    width: parent.width

    signal controlGroupClicked(bool checked)
    signal controlGroupChecked(bool checked)
    signal inputChanged(var data)

    ColumnLayout {
        id: controlColumn
        anchors.fill: parent
        spacing: 10
        width: parent.width

        RowLayout {
            id: switchRow
            Layout.fillWidth: true
            visible: controlGroup.showSwitch || controlGroup.label !== ""
            spacing: 1
            Layout.bottomMargin: 0
            height: Math.max(controlSwitch.implicitHeight, titleLabel.implicitHeight) + 3

            PCheckBox {
                id: controlSwitch
                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                onCheckedChanged: {
                    controlGroup.enabled = controlSwitch.checked
                    if (controlGroup.data) {
                        controlGroup.data.enabled = controlSwitch.checked
                    }

                    controlGroup.controlGroupChecked(controlSwitch.checked)
                }
                visible: controlGroup.showSwitch
            }


            Text {
                id: titleLabel
                text: controlGroup.label
                font.pixelSize: 12
                color: controlGroup.enabled ? "#E8E8CF" : "#A0A0A0"
                Layout.alignment: Qt.AlignVCenter
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
                spacing: 5
                enabled: controlGroup.enabled
            }
        }

    }

}
