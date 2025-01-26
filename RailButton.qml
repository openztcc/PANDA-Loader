import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Effects

Item {
    id: root
    width: parent ? parent.width : 80
    height: 40

    // padding

    required property string text
    required property string icon
    property string fg  // Foreground (text/icon color)
    property string bg  // Background color for rounded icon area

    signal clicked()

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()

        Column {
            anchors.centerIn: parent
            spacing: 2

            // Rounded icon background
            Rectangle {
                width: 56
                height: 30
                radius: 16
                color: hoverArea.containsMouse ? Qt.lighter("#3a4b3a", 1.2) : "transparent"
                // animate
                Behavior on color { ColorAnimation { duration: 70 } }

                anchors.horizontalCenter: parent.horizontalCenter

                Image {
                    id: railImg
                    anchors.centerIn: parent
                    source: root.icon
                    width: 24
                    height: 24
                    fillMode: Image.PreserveAspectFit
                    // color: root.fg ? root.fg : "#d4e8d1"
                }

                MultiEffect {
                    source: railImg
                    anchors.fill: railImg
                    colorization: 1.0
                    colorizationColor: "#d4e8d1"
                }
            }

            // Label below the icon
            Label {
                text: root.text
                font.pixelSize: 10
                color: root.fg ? root.fg : "#d4e8d1"
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
