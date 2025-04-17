import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: check
    implicitHeight: Math.max(contentRow.height, 30)
    implicitWidth: contentRow.width + 10
    width: implicitWidth
    height: implicitHeight
    property alias checked: pCheckBox.checked
    property alias text: labelText.text
    property var bg: "#627D58"

    signal toggled(bool checked)
    signal checkChanged(bool checked)
    signal dataChanged(var data)

    CheckBox {
        id: pCheckBox
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height
        
        onToggled: {
            check.toggled(checked)
        }

        onCheckedChanged: {
            check.checkChanged(checked)
        }

        indicator: Item {}
        background: Item {}

        contentItem: Item {
            width: contentRow.width
            height: contentRow.height
        
            Row {
                id: contentRow
                anchors.verticalCenter: parent.verticalCenter
                spacing: 6
                anchors.left: parent.left

                Rectangle {
                    id: checkBox
                    width: 15
                    height: 15
                    radius: 3
                    color: {
                        if (pCheckBox.checked) {
                            if (pCheckBox.hovered) {
                                return Qt.darker(check.bg, 1.25)
                            } 
                            else {
                                return Qt.darker(check.bg, 1.2)
                            }
                        } else {
                            if (pCheckBox.hovered) {
                                return Qt.darker(check.bg, 1.05)
                            } else {
                                return check.bg
                            }
                        }
                    }
                    border.width: 1
                    border.color: Qt.darker(color, 1.2)

                    SvgIcon {
                        anchors.centerIn: parent
                        icon: "qrc:/icons/check.svg"
                        iconWidth: 12
                        iconHeight: 12
                        color: pCheckBox.checked ? "#E8E8CF" : "#A0A0A0"
                        visible: pCheckBox.checked
                    }
                }

                Text {
                    id: labelText
                    text: ""
                    color: pCheckBox.checked ? "#E8E8CF" : "#A0A0A0"
                    font.pixelSize: 12
                    anchors.verticalCenter: parent.verticalCenter
                }

                // MouseArea {
                //     id: mouseArea
                //     anchors.fill: parent
                //     hoverEnabled: true
                //     cursorShape: Qt.PointingHandCursor
                //     onClicked: pCheckBox.toggle()
                // }
                
            }
        }
    }
}
        // MouseArea {
        //     id: checkBoxMouseArea
        //     anchors.fill: pCheckBox
        //     hoverEnabled: true
        //     propagateComposedEvents: true
        //     onEntered: {
        //         checkIcon.color = "#E8E8CF"
        //     }
        //     onExited: {
        //         checkIcon.color = pCheckBox.checked ? "#E8E8CF" : "#A0A0A0"
        //     }

        //     onClicked: {
        //         pCheckBox.checked = !pCheckBox.checked
        //         checkIcon.visible = pCheckBox.checked
        //     }
        // }
