import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: searchBar
    Layout.fillWidth: true
    height: implicitHeight

    property bool isTagOpen: false
    property string activeFilter: ""

    // signals
    signal filterBy(filter: string)
    signal searchTextChanged(text: string)

    MouseArea {
        id: backgroundMouseArea
        anchors.fill: searchBar
        propagateComposedEvents: true // Allow clicks to pass through when needed
        onClicked: {
            searchField.focus = false;
        }
    }

    PTextField {
        id: searchField
        placeholderText: ""
        placeholderColor: "#E8E8CF"
        anchors.fill: parent
        bg: "#627D58"
        leftPadding: searchBar.isTagOpen ? activeFilterTag.width + 15 : 8
        radius: 0
        trailingIcon: "qrc:/icons/search.svg"
        border: 0

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

        onTextChange: (text) => {
            if (text === "by:" || text === "category:" || text === "disabled:" || text === "enabled:") {
                console.log("Found tag: " + text + " in SearchBar.qml")
                searchBar.activeFilter = text;
                searchBar.isTagOpen = true;

                // send signals for filtering
                if (text === "by:") {
                    searchBar.filterBy("author");
                } else {
                    searchBar.filterBy(text.slice(0, -1)); // remove the colon
                }

                searchField.text = "";

                // wait until frame update to change padding
                // TODO: remove...might not be effective
                Qt.callLater(() => {
                    searchField.leftPadding = activeFilterTag.width + 15;
                });
            } 
            else {
                // emit signal for text change
                searchBar.searchTextChanged(text);
            }
        }

        onCleared: {
            searchBar.activeFilter = ""
            searchBar.isTagOpen = false
            searchField.leftPadding = 8
            searchBar.searchTextChanged("");
            searchBar.filterBy("");
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
        onPressed: (event) => {
            console.log("Key event logged at SearchBar.qml")
            // Allow Escape key to clear filter
            if (event.key === Qt.Key_Escape) {
                console.log("Escape key hit at SearchBar.qml")
                searchBar.isTagOpen = false
                searchBar.activeFilter = ""
                event.accepted = true
                searchField.leftPadding = 8
                // remove focus from search field
                searchField.focus = false
                // update signals
                searchBar.searchTextChanged("");
                searchBar.filterBy("");
            }

            // Delete filter tag when backspace is pressed
            if (event.key === Qt.Key_Backspace && searchBar.isTagOpen && searchField.text === "") {
                console.log("Attempted to delete tag at SearchBar.qml with key event.")
                searchBar.isTagOpen = false
                searchBar.activeFilter = ""
                searchField.leftPadding = 8
                // update signals
                searchBar.filterBy("");
            }
        }

        // MouseArea {
        //     anchors.fill: parent
        //     cursorShape: Qt.PointingHandCursor
        //     onClicked: console.log("Search clicked")
        // }
    }
}
