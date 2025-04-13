import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: check
    implicitHeight: 30
    implicitWidth: 30
    width: implicitWidth
    height: implicitHeight
    property alias checked: pCheckBox.checked
    property alias text: pCheckBox.text
    property var bg: "#627D58"

    CheckBox {
        id: pCheckBox
        anchors.fill: parent
        indicator: {}

        contentItem: Row {
            anchors.verticalCenter: parent.verticalCenter
            spacing: 6

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
                anchors.left: parent.left

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
                text: pCheckBox.text
                color: pCheckBox.checked ? "#E8E8CF" : "#A0A0A0"
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: 10
                anchors.bottomMargin: 7
                anchors.left: checkBox.right
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: pCheckBox.toggle()
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

}
