import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    Layout.preferredHeight: 40
    Layout.fillWidth: true

    TextField {
        id: searchField
        placeholderText: "Search"
        // Material.background: "#f7fbf2"
        // Material.foreground: "#424940"
        placeholderTextColor: "#424940"
        anchors.fill: parent
        readOnly: false

        background: Rectangle {
            color: "#f7fbf2"
            radius: 0
        }

        onFocusChanged: {
            if (focus) {
                searchField.placeholderText = ""
                console.log("Search clicked")
            } else {
                if (searchField.text == "") {
                    searchField.placeholderText = "Search"
                }
            }
        }

        onTextChanged: {
                    if (searchField.text == "by:" ||
                        searchField.text == "category:" ||
                        searchField.text == "disabled:" ||
                        searchField.text == "enabled:") {
                        searchField.currentBgColor = "#424940"  // Change the property value
                    } else {
                        searchField.currentBgColor = "#f7fbf2"  // Reset to default
                    }
                }

        // MouseArea {
        //     anchors.fill: parent
        //     cursorShape: Qt.PointingHandCursor
        //     onClicked: console.log("Search clicked")
        // }
    }
}
