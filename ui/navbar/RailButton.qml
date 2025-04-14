import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: railButton
    width: parent.width
    height: buttonContent.height

    required property string text
    required property string icon
    property string fg: "#E8E8CF"
    property string bg: "transparent"
    property string color: "#34472D"
    property bool selected: false
    property bool topButton: false

    signal clicked()

    Rectangle {
        id: buttonContent
        width: parent.width
        height: colId.height + 20
        color: if (hoverArea.containsPress) {
            "#627D58"
        } else if (hoverArea.containsMouse || railButton.selected) {
            Qt.darker("#627D58", 1.1)
        } else {
            railButton.bg
        }



        ColumnLayout {
            id: colId
            anchors.centerIn: parent
            width: parent.width


            SvgIcon {
                icon: railButton.icon
                color: if (hoverArea.containsPress || railButton.selected) {
                    "#FED286"
                } else if (hoverArea.containsMouse) {
                    Qt.darker("#FED286", 1.1)
                } else {
                    railButton.fg
                }
                Layout.alignment: Qt.AlignHCenter

            }

            // Label below the icon
            Label {
                text: railButton.text
                font.pixelSize: 10
                color: railButton.fg
                Layout.alignment: Qt.AlignHCenter
            }
        }
        Rectangle {
            anchors.bottom: parent.bottom
            height: 1
            color: Qt.darker(railButton.color, 1.2)
            width: parent.width
        }

        Rectangle {
            anchors.top: parent.top
            height: railButton.topButton ? 1 : 0
            color: Qt.darker(railButton.color, 1.2)
            width: parent.width
        }
    }

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: railButton.clicked()
        cursorShape: Qt.PointingHandCursor

    }
}

