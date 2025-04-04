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
                icon: infoPaneItem.iconImg
                iconWidth: 24
                iconHeight: 24
                bgWidth: 24
                bgHeight: 24
                color: "#424940"
                Layout.alignment: Qt.AlignTop
                Layout.leftMargin: 15
                Layout.topMargin: 15
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 3

                Label {
                    text: infoPaneItem.fieldName
                    font.pixelSize: 10
                }

                Label {
                    text: infoPaneItem.innerComponent ? infoPaneItem.innerComponent : "No text"
                    font.pixelSize: 12
                    color: "#424940"
                    clip: true
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }
}
