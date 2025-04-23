import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls

// generic content area for the main application

Rectangle {
    id: mainContent
    property var offset: null
    default property alias content: contentContainer.children
    anchors.fill: parent
    width: parent.width - 70
    height: parent.height
    color: "#34472D"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            id: contentContainer
            Layout.preferredHeight: implicitHeight - statusBar.height
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            spacing: 0

        }
        // status bar
        Rectangle {
            id: statusBar
            z: 10
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            color: "#4F6846"
            Layout.alignment: Qt.AlignBottom
            Layout.margins: 0

            Rectangle {
                anchors.top: parent.top
                color: Qt.darker(statusBar.color, 1.2)
                height: 1
                width: parent.width
            }

            // Text {
            //     id: statusLabel
            //     text: "Status: " + statusLabel.model
            //     anchors.left: parent.left
            //     anchors.leftMargin: 10
            //     anchors.verticalCenter: parent.verticalCenter
            //     color: "#424940"
            //     font.pixelSize: 10

            //     property string model: "No status to show"
            // }
        }
    }

}
