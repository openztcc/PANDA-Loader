import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: section

    property string title: ""
    property color textColor: "white"
    property real spacing: 10
    default property alias content: contentArea.data

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: section.spacing

        // Optional heading
        Label {
            text: section.title
            visible: section.title !== ""
            color: section.textColor
            font.bold: true
            font.pixelSize: 14
        }

        // User-defined content goes here
        Item {
            id: contentArea
            Layout.fillWidth: true
        }
    }
}
