import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: check
    height: 30
    width: 30
    property alias checked: pCheckBox.checked
    property alias implicitHeight: pCheckBox.implicitHeight
    property var bg: "#627D58"

    CheckBox {
        id: pCheckBox

        indicator: Rectangle {
            width: 15
            height: 15
            anchors.verticalCenter: pCheckBox.verticalCenter
            radius: 3
            border.width: 1
            border.color: Qt.darker(check.bg, 1.2)
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

            SvgIcon {
                id: checkIcon
                anchors.centerIn: parent
                iconWidth: 14
                iconHeight: 14
                icon: "qrc:/icons/check.svg"
                color: pCheckBox.checked ? "#E8E8CF" : "#A0A0A0"
                visible: pCheckBox.checked
            }

            // onClicked: {
            //     if (pCheckBox.checked) {
            //         checkIcon.icon = "qrc:/icons/close.svg"
            //         checkIcon.visible = true
            //     } else {
            //         checkIcon.visible = false
            //     }
            // }
        }

        contentItem: {}
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
