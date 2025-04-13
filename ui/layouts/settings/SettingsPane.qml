import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: section
    property string title: ""
    property color textColor: "white"
    property real spacing: 10
    default property alias content: contentContainer.children
    anchors.fill: parent

    Flickable {
        id: flick
        anchors.fill: parent
        width: parent.width
        contentWidth: width
        contentHeight: contentContainer.implicitHeight
        clip: false
        boundsBehavior: Flickable.StopAtBounds
        Layout.topMargin: 6
        Layout.bottomMargin: 6
        
        ColumnLayout {
            id: contentContainer
            width: flick.width
            spacing: section.spacing

            Label {
                text: section.title
                visible: section.title !== ""
                color: section.textColor
                font.bold: true
                font.pixelSize: 14
                
                Layout.topMargin: 6
            }
        }
    }
}

