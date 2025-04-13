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

    RowLayout {
        anchors.fill: parent
        spacing: 16

        Item {
            Layout.fillWidth: true
        }

        // Appbar label
        Label {
            id: toolbarLabel
            text: "PANDA Loader"
            font.pixelSize: 14
            color: "#424940"
            Layout.alignment: Qt.AlignHCenter
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
                width: 40
                height: 40
                anchors.verticalCenter: parent.verticalCenter
                onClicked: toolbar.offset.open()
            }

            MultiEffect {
                source: notif
                anchors.fill: notif
                colorization: 1.0
                colorizationColor: "#424940"
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: console.log("Notifications clicked")
            }
        }

    }
}
