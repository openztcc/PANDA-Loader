import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: railButton
    width: parent ? parent.width : 80
    height: 40

    // padding

    required property string text
    required property string icon
    property string fg: "#E8E8CF"  // Foreground (text/icon color)
    property string bg: "transparent"  // Background color for rounded icon area
    property string color: "#34472D"  // Default icon bg color

    signal clicked()

    Column {
        anchors.centerIn: parent
        spacing: 2

        // Rounded icon background
        Rectangle {
            width: 56
            height: 30
            radius: 5
            color: if (hoverArea.containsPress) {
                "#627D58"
            } else if (hoverArea.containsMouse) {
                Qt.darker("#627D58", 1.1)
            } else {
                railButton.bg
            }

            SvgIcon {
                bgWidth: parent.width
                bgHeight: parent.height
                icon: railButton.icon
                color: if (hoverArea.containsPress) {
                    "#FED286"
                } else if (hoverArea.containsMouse) {
                    Qt.darker("#FED286", 1.1)
                } else {
                    railButton.fg
                }
            }
        }

        // Label below the icon
        Label {
            text: railButton.text
            font.pixelSize: 10
            color: railButton.fg
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    // hover area config for the button
    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: railButton.clicked()
        cursorShape: Qt.PointingHandCursor

    }
}

