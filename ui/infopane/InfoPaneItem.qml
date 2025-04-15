import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Rectangle {
    id: infoPaneItem
    color: "#506648"
    radius: 0
    property string fieldName: "No field name"
    property var innerComponent: null
    property var iconImg: null
    Layout.fillWidth: true
    Layout.preferredHeight: implicitHeight
    implicitWidth: contentRow.implicitWidth
    implicitHeight: contentRow.implicitHeight + bottomBorder.height

    RowLayout {
        id: contentRow
        anchors.fill: parent
        spacing: 8

        Text {
            id: fieldName
            text: infoPaneItem.fieldName
            font.pixelSize: 10
            color: "#E8E8CF"
            wrapMode: Text.WordWrap
            Layout.preferredWidth: parent.width * 0.3
            Layout.fillHeight: true
            padding: 10
        }

        Text {
            id: valueName
            text: infoPaneItem.innerComponent
            wrapMode: Text.WordWrap
            font.pixelSize: 12
            color: "#ffffff"
            Layout.fillWidth: true
            Layout.fillHeight: true
            padding: 10
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
