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
        color: "#e5e9e1"
        radius: 0
        anchors.fill: parent
        width: parent.width
        
        // Item to create a padding
        ColumnLayout {
            anchors.margins: 10
            anchors.fill: parent
            Layout.fillWidth: true
            spacing: 10

            // mod title
            Text {
                id: modDetailsText
                text: infoPane.targetComponent ? infoPane.targetComponent.modTitle : "No mod selected"
                color: "#424940"
                font.pixelSize: 16
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
                text: infoPane.targetComponent ? infoPane.targetComponent.modDescription : "No description"
                color: "#424940"
                font.pixelSize: 12
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                lineHeight: 14
                lineHeightMode: Text.FixedHeight
                Layout.alignment: Qt.AlignTop
            }

            // authors
            InfoPaneItem {
                fieldName: "Authors"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.modAuthor : "No authors"
                iconImg: "qrc:/icons/author.svg"
                Layout.fillWidth: true
            }

            // mod path
            InfoPaneItem {
                fieldName: "Path"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.modLocation : "No path"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // File Name
            InfoPaneItem {
                fieldName: "File Name"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.modFilename : "No file name"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // mod ID
            InfoPaneItem {
                fieldName: "Mod ID"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.modId : "No ID"
                iconImg: "qrc:/icons/path.svg"
                Layout.fillWidth: true
            }

            // enabled
            InfoPaneItem {
                fieldName: "Status"
                innerComponent: infoPane.targetComponent ? infoPane.targetComponent.modEnabled ? "Enabled" : "Disabled" : "No status"
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
