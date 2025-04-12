import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: section
    property string title: ""
    property color textColor: "white"
    property real spacing: 30
    default property alias content: contentContainer.data
    anchors.fill: parent

    Flickable {
        id: flick
        anchors.fill: parent
        width: parent.width
        contentWidth: flick.width
        contentHeight: layout.implicitHeight
        clip: true
        
        default property alias content: contentContainer.children
        ColumnLayout {
            id: contentContainer
            width: flick.width
            spacing: section.spacing
            Layout.fillWidth: true

            Label {
                text: section.title
                visible: section.title !== ""
                color: section.textColor
                font.bold: true
                font.pixelSize: 14
                Layout.margins: 4
            }
        }
    }
}

