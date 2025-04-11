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
    property string fg: "#d4e8d1"  // Foreground (text/icon color)
    property string bg: "transparent"  // Background color for rounded icon area
    property string color: "#3a4b3a"  // Default icon bg color

    signal clicked()

    Column {
        anchors.centerIn: parent
        spacing: 2

        // Rounded icon background
        Rectangle {
            width: 56
            height: 30
            radius: 16
            color: if (hoverArea.containsPress) {
                return Qt.lighter(railButton.color, 1.2)
            } else if (hoverArea.containsMouse) {
                return Qt.darker(railButton.color, 1.1)
            } else {
                return railButton.bg
            }
            // animate
            Behavior on color { ColorAnimation { duration: 70 } }


            SvgIcon {
                bgWidth: parent.width
                bgHeight: parent.height
                icon: railButton.icon
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

