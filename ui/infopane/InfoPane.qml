import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

Item {
    id: infoPane
    property string text: "No text"
    property var targetComponent: null
    implicitWidth: 300
    Layout.fillHeight: true

    Rectangle {
        id: modDetailsPane
        color: "#57704E"
        radius: 0
        anchors.fill: parent
        width: parent.width
        
        // Item to create a padding
        ColumnLayout {
            anchors.margins: 24
            anchors.fill: parent
            Layout.fillWidth: true

            ColumnLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                spacing: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                Layout.bottomMargin: 20

                // mod title
                Text {
                    id: modDetailsText
                    text: infoPane.targetComponent ? infoPane.targetComponent.title : "No mod selected"
                    color: "#ffffff"
                    font.pixelSize: 20
                    font.bold: true
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    lineHeight: 18
                    lineHeightMode: Text.FixedHeight
                    Layout.alignment: Qt.AlignTop
                }

                // description
                Text {
                    id: modDetailsDesc
                    text: infoPane.targetComponent ? infoPane.targetComponent.description : "No description"
                    color: "#ffffff"
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.preferredHeight: contentHeight
                    lineHeight: 14
                    lineHeightMode: Text.FixedHeight
                    Layout.alignment: Qt.AlignTop
                }
            }

            // authors
            InfoPaneItem {
                fieldName: "Authors"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.authors : "No authors"
                iconImg: "qrc:/icons/author.svg"
                Layout.fillWidth: true
            }

            // mod path
            InfoPaneItem {
                fieldName: "Path"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.location : "No path"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // File Name
            InfoPaneItem {
                fieldName: "File Name"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.filename : "No file name"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // mod ID
            InfoPaneItem {
                fieldName: "Mod ID"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.id : "No ID"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // enabled
            InfoPaneItem {
                fieldName: "Status"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.enabled ? "Enabled" : "Disabled" : "No status"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // without this content is spread too wide
            Item {
                Layout.fillHeight: true
            }
        }

    }

    // Connections {
    //     target: infoPane.targetComponent
    //     onSelectedMod: (mod) => {
    //         modDetailsText.text = mod.modTitle
    //         console.log("Selected mod: " + mod.modTitle)
    //     }
    // }
}
