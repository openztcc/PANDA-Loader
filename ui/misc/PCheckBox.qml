import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects

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
                    width: 16
                    height: 18
                    radius: 3
                    color: "#627D5800"
                    border.width: 1
                    border.color: Qt.darker(check.bg, 1.2)

                    DropShadow  {
                        anchors.fill: checkBox
                        radius: 2.0
                        color: "#80000000"
                        verticalOffset: 1.5
                        horizontalOffset: 0
                    }

                    Rectangle {
                        id: innerBox
                        width: 16
                        height: 16
                        anchors.top: parent.top
                        border.width: 1
                        border.color: Qt.lighter(color, 1.2)
                        radius: 3
                        color: {
                            if (pCheckBox.checked) {
                                return Qt.darker("#FED286", 1.1)
                            } else {
                                if (pCheckBox.hovered) {
                                    return Qt.lighter(check.bg, 1.15)
                                } else {
                                    return check.bg
                                }
                            }
                        }
                    }


                    SvgIcon {
                        anchors.centerIn: parent
                        icon: "qrc:/icons/check.svg"
                        iconWidth: 12
                        iconHeight: 12
                        color: check.bg
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
