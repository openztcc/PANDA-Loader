import QtQuick 2.15
import QtQuick.Layouts 1.15

// generic content area for the main application

Item {
    id: mainContent
    property var offset: null
    default property alias content: contentContainer.children
    anchors.fill: parent
    width: parent.width - 70
    height: parent.height - 40

    ColumnLayout {
        anchors.fill: parent

        ColumnLayout {
            id: contentContainer
            Layout.preferredHeight: implicitHeight - statusBar.height
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop

        }
        // status bar
        Rectangle {
            id: statusBar
            z: 10
            Layout.preferredHeight: 30
            Layout.fillWidth: true
            color: "#4F6846"
            Layout.alignment: Qt.AlignBottom

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
