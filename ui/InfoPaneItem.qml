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
    height: 50

    Rectangle {
        id: infoPaneItemRoot
        color: "#f7fbf2"
        radius: 0
        anchors.fill: parent

        RowLayout {
            id: infoPaneItemLayout
            spacing: 12
            anchors.fill: parent

            SvgIcon {
                id: infoPaneItemIcon
                icon: iconImg
                iconWidth: 24
                iconHeight: 24
                bgWidth: 24
                bgHeight: 24
                color: "#d4e8d1"
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 3

                Label {
                    text: fieldName
                    font.pixelSize: 10
                }

                Label {
                    text: innerComponent ? innerComponent : "No text"
                    font.pixelSize: 12
                    color: "#424940"
                }
            }
        }
    }
}
