import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: infoPaneItem
    property string fieldName: "No field name"
    property var innerComponent: null
    property var iconImg: null
    width: parent ? parent.width : 300
    Layout.preferredHeight: 50

    Rectangle {
        id: infoPaneItemRoot
        color: "#506648"
        anchors.fill: parent

        RowLayout {
            id: infoPaneItemLayout
            spacing: 12
            anchors.fill: parent
            anchors.leftMargin: 15

            // property i.e. author
            Label {
                text: infoPaneItem.fieldName
                font.pixelSize: 10
                color: "#E8E8CF"
            }

            // value i.e. goosifer
            Text {
                text: infoPaneItem.innerComponent
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                font.pixelSize: 12
                color: "#ffffff"
                clip: true
            }
        }

        Rectangle {
            id: bottomBorder
            color: Qt.darker("#506648", 1.2)
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }
}
