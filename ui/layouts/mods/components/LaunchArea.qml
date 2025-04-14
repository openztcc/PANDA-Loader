import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import PandaUI 1.0

// launch area
Rectangle {
    id: launchArea
    Layout.fillWidth: true
    Layout.preferredHeight: 150
    Layout.alignment: Qt.AlignTop

    Image {
        id: bgImage
        source: "qrc:/images/launch_bg.jpg"
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop

        MultiEffect {
            source: bgImage
            anchors.fill: bgImage
            saturation: 0.0
        }
    }

    RowLayout {
        width: parent.width
        height: 70
        spacing: 8
        anchors.bottom: parent.bottom

        // Spacer to push buttons to the right
        Item { Layout.fillWidth: true }  

        Button {
            Material.background: "#FED286"
            Material.foreground: "#4D523B"
            text: "Launch"

            background: Rectangle {
                color: "#FED286"
                radius: 5
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: PState.launchZT()
            }


        }

        Button {
            text: "Install OpenZT"
            Material.foreground: "#4D523B"
            Material.background: "#FEB886"

            background: Rectangle {
                color: "#FEB886"
                radius: 5
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: console.log("Install OpenZT clicked")
            }
        }

        Item {
            Layout.preferredWidth: 12
        }
    }

}
