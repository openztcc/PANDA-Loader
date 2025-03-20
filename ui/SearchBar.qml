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

    // signals
    signal filterByAuthor()
    signal filterByCategory()
    signal filterByDisabled()
    signal filterByEnabled()

    MouseArea {
        id: backgroundMouseArea
        anchors.fill: parent.parent
        propagateComposedEvents: true // Allow clicks to pass through when needed
        onClicked: {
            searchField.focus = false;
        }
    }

    TextField {
        id: searchField
        placeholderText: "Search"
        // Material.background: "#f7fbf2"
        // Material.foreground: "#424940"
        placeholderTextColor: "#424940"
        anchors.fill: parent
        readOnly: false
        leftPadding: searchBar.isTagOpen ? activeFilterTag.width + 15 : 8

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

            FilterTag {
                id: activeFilterTag
                filter: searchBar.activeFilter
                visible: searchBar.isTagOpen
                anchors.verticalCenter: parent.verticalCenter
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
            if (text === "by:" || text === "category:" || text === "disabled:" || text === "enabled:") {
                searchBar.activeFilter = text;
                searchBar.isTagOpen = true;
                searchField.text = "";

                // wait until frame update to change padding
                // TODO: remove...might not be effective
                Qt.callLater(() => {
                    searchField.leftPadding = activeFilterTag.width + 15;
                });
            }

            // emit signals for filtering
            if (text === "by:") {
                searchBar.filterByAuthor();
            } else if (text === "category:") {
                searchBar.filterByCategory();
            } else if (text === "disabled:") {
                searchBar.filterByDisabled();
            } else if (text === "enabled:") {
                searchBar.filterByEnabled();
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
            if (event.key === Qt.Key_Escape) {
                searchBar.isTagOpen = false
                searchBar.activeFilter = ""
                searchField.text = ""
                event.accepted = true
                searchField.leftPadding = 8
                // remove focus from search field
                searchField.focus = false
            }

            // Delete filter tag when backspace is pressed
            if (event.key === Qt.Key_Backspace && searchBar.isTagOpen && searchField.text === "") {
                searchBar.isTagOpen = false
                searchBar.activeFilter = ""
                searchField.leftPadding = 8
            }
        }

        // MouseArea {
        //     anchors.fill: parent
        //     cursorShape: Qt.PointingHandCursor
        //     onClicked: console.log("Search clicked")
        // }
    }
}
