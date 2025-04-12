import QtQuick 
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: clear

    property var textField: null
    property var fg: textField.fg
    property alias bg: clearBg.bg

    width: 35
    height: textField.height - 7

    Button {
        id: clearButton
        property var buttonText: clear.textField.text
        property var bg: clear.textField.bg
        property var fg: clear.textField.fg

        anchors.fill: parent
        flat: true
        background: Rectangle {
            property var bg: clearButton.bg
            property bool hovered: false

            id: clearBg
            color: hovered ? Qt.darker(bg, 1.05) : bg
            anchors.fill: parent
        }
        visible: buttonText.length > 0
        contentItem: SvgIcon {
            property var fg: clear.textField.fg
            icon: "qrc:/icons/close.svg"
            color: fg
            iconWidth: 15
            iconHeight: 15
        }
        onClicked: {
            console.log("Clear clicked")
            clear.textField.text = ""
            clear.textField.focus = false
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton
        cursorShape: Qt.PointingHandCursor
        propagateComposedEvents: true
        onEntered: {
            clearBg.hovered = true
        }
        onExited: {
            clearBg.hovered = false
        }
        z: 1
    }
}

            // Button {
            //     id: clearButton
            //     height: textField.height - 7 // to avoid overlap with border    
            //     width: 35
            //     flat: true
            //     background: Rectangle {
            //         id: clearBg
            //         property bool hovered: false
            //         color: clearButton.hovered ? Qt.darker(rField.bg, 1.05) : rField.bg
            //         anchors.fill: parent

            //     }
            //     text: ""
            //     anchors.verticalCenter: parent.verticalCenter
            //     anchors.right: {
            //         if (textField.isFileBrowser) {
            //             browseFilesButton.left
            //         } else {
            //             textField.right
            //         }
            //     }
            //     visible: textField.text.length > 0
            //     contentItem: SvgIcon {
            //         id: clearIcon
            //         icon: "qrc:/icons/close.svg"
            //         color: rField.placeholderColor
            //         iconWidth: 15
            //         iconHeight: 15
            //     }
            //     onClicked: {
            //         console.log("Clear clicked")
            //         textField.text = ""
            //         textField.focus = false
            //     }

            //     MouseArea {
            //         anchors.fill: parent
            //         hoverEnabled: true
            //         acceptedButtons: Qt.NoButton
            //         propagateComposedEvents: true
            //         z: 1
            //         cursorShape: Qt.PointingHandCursor
            //     }
            // }
