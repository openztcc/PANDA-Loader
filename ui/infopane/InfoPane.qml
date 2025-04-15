import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

pragma ComponentBehavior: Bound

Rectangle {
    id: infoPane
    color: "#57704E"
    property string text: "No text"
    property var targetComponent: null
    Layout.fillHeight: true
    Layout.fillWidth: true

    // Border elements
    Rectangle {
      anchors.left: parent.left
      height: parent.height
      color: Qt.darker(parent.color, 1.2)
      width: 1
    }

    Rectangle {
      anchors.top: parent.top
      width: parent.width
      color: Qt.darker(parent.color, 1.2)
      height: 1
    }

    Rectangle {
      id: modDetailsPane
      radius: 0
      anchors.fill: parent
      anchors.margins: 15
      color: parent.color

      ScrollView {
          id: scrollView
          anchors.fill: parent
          clip: true
          ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

          ColumnLayout {
              id: metaLayout
              width: scrollView.width
              spacing: 0

              // Mod title
              Text {
                  id: modDetailsText
                  text: infoPane.targetComponent ? infoPane.targetComponent.title : "No mod selected"
                  color: "#ffffff"
                  font.pixelSize: 20
                  font.bold: true
                  width: metaLayout.width
                  Layout.fillWidth: true
                  lineHeight: 18
                  lineHeightMode: Text.FixedHeight
                  wrapMode: Text.WordWrap
                  padding: 10
              }

              // Description
              Text {
                  id: modDetailsDesc
                  text: infoPane.targetComponent ? infoPane.targetComponent.description : "No description"
                  color: "#ffffff"
                  font.pixelSize: 14
                  wrapMode: Text.WordWrap
                  width: metaLayout.width
                  Layout.fillWidth: true
                  lineHeight: 14
                  lineHeightMode: Text.FixedHeight
                  rightPadding: 10
                  leftPadding: 10
                  topPadding: 10
                  bottomPadding: 20
              }

              // Authors
              InfoPaneItem {
                  fieldName: "Authors"
                  innerComponent: infoPane.targetComponent ? infoPane.targetComponent.authors : "No authors"
                  Layout.fillWidth: true
              }

              // Mod path
              InfoPaneItem {
                  fieldName: "Path"
                  innerComponent: infoPane.targetComponent ? infoPane.targetComponent.location : "No path"
                  Layout.fillWidth: true
              }

              // File Name
              InfoPaneItem {
                  fieldName: "File Name"
                  innerComponent: infoPane.targetComponent ? infoPane.targetComponent.filename : "No file name"
                  Layout.fillWidth: true
              }

              // Mod ID
              InfoPaneItem {
                  fieldName: "Mod ID"
                  innerComponent: infoPane.targetComponent ? infoPane.targetComponent.id : "No ID"
                  Layout.fillWidth: true
              }

              // Enabled
              InfoPaneItem {
                  fieldName: "Status"
                  innerComponent: infoPane.targetComponent ?
                      (infoPane.targetComponent.enabled ? "Enabled" : "Disabled") : "No status"
                  Layout.fillWidth: true
              }
                // spacer or else it sinks to bottom
              Item {
                  Layout.fillHeight: true
              }
          }
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

