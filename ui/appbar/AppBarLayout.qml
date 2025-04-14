import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Effects

// Appbar
Rectangle {
    id: toolbar
    property var offset: null
    anchors.fill: parent
    color: "#34472D"

    Rectangle {
        color: Qt.darker(parent.color, 1.2)
        height: 1
        width: parent.width
        anchors.bottom: parent.bottom
    }

    RowLayout {
        anchors.fill: parent
        spacing: 16

        Item {
            Layout.fillWidth: true
        }

        // Appbar label
        Row {
            id: appbarLabel
            spacing: 3
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Text {
                id: toolbarLabel
                text: "PANDA"
                font.pixelSize: 14
                color: "#E8E8CF"
                font.bold: true
            }

            Text {
                id: toolbarLabel2
                text: "Loader"
                font.pixelSize: 14
                color: "#FED286"
                font.bold: true
            }
        }

        Item {
            Layout.fillWidth: true
        }

        // Notification button
        Item {
            Layout.alignment: Qt.AlignRight
            Layout.fillHeight: true
            Layout.rightMargin: 10
            implicitWidth: 40
            ToolButton {
                id: notif
                icon.source: "qrc:/icons/notifications.svg"
                Layout.alignment: Qt.AlignRight
                width: 30
                height: 30
                anchors.verticalCenter: parent.verticalCenter
                onClicked: toolbar.offset.open()
            }

            MultiEffect {
                source: notif
                anchors.fill: notif
                colorization: 1.0
                colorizationColor: "#E8E8CF"
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: console.log("Notifications clicked")
            }
        }

    }
}
