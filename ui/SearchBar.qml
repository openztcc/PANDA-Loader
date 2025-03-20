import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: searchBar
    Layout.preferredHeight: 40
    Layout.fillWidth: true

    property bool isTagOpen: false
    property string activeFilter: ""

    TextField {
        id: searchField
        placeholderText: "Search"
        // Material.background: "#f7fbf2"
        // Material.foreground: "#424940"
        placeholderTextColor: "#424940"
        anchors.fill: parent
        readOnly: false
        leftPadding: searchBar.isTagOpen ? (activeFilterTag.width + 38) : 8

        background: Rectangle {
            id: searchFieldBg
            color: "#f7fbf2"
            radius: 0
        }

        Row {
            id: textContainer
            spacing: 5
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5

            // Filter Tag inside the TextField
            FilterTag {
                id: activeFilterTag
                filter: searchBar.activeFilter
                visible: searchBar.isTagOpen
                anchors.verticalCenter: parent.verticalCenter
            }

            TextInput {
                id: userInput
                color: "#424940"
                font.pixelSize: 16
                width: parent.width - (searchBar.isTagOpen ? activeFilterTag.width + 38 : 0)
                focus: true
                text: ""
            }
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
            if (text === "by:") {
                searchBar.activeFilter = "by:";
                searchBar.isTagOpen = true;
                searchField.leftPadding = activeFilterTag.width + 38
                searchField.text = ""
            }
            else if (text === "category:") {
                searchBar.activeFilter = "category:";
                searchBar.isTagOpen = true;
                searchField.text = ""
                searchField.leftPadding = activeFilterTag.width + 38
            }
            else if (text === "disabled:") {
                searchBar.activeFilter = "disabled:";
                searchBar.isTagOpen = true;
                searchField.text = ""
                searchField.leftPadding = activeFilterTag.width + 38
            }
            else if (text === "enabled:") {
                searchBar.activeFilter = "enabled:";
                searchBar.isTagOpen = true;
                searchField.text = ""
                searchField.leftPadding = activeFilterTag.width + 38
            }
        }



        // timer to allow backspace to work without immediately clearing filter
        // Timer {
        //     id: clearFilterTimer
        //     interval: 300 // wait a o see if user is typing
        //     repeat: false
        //     onTriggered: {
        //         if (searchField.text == "") {
        //             searchBar.isTagOpen = false
        //             searchBar.activeFilter = ""
        //         }
        //     }
        // }

        // Key handling
        Keys.onPressed: function(event) {
            // Allow Escape key to clear filter
            if (event.key === Qt.Key_Escape && searchBar.isTagOpen) {
                searchBar.isTagOpen = false
                searchBar.activeFilter = ""
                searchField.text = ""
                event.accepted = true
                searchField.leftPadding = 8
                // activeFilterTag.filter = ""
            }
        }

        // MouseArea {
        //     anchors.fill: parent
        //     cursorShape: Qt.PointingHandCursor
        //     onClicked: console.log("Search clicked")
        // }
    }
}
