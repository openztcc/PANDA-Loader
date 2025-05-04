import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import PandaUI 1.0

Item {
    id: searchBar
    Layout.fillWidth: true
    height: implicitHeight

    property bool isTagOpen: false
    property string activeTag: ""
    property string activeFilter: ""

    // signals
    signal filterBy(filter: string, search: string)
    signal searchTextChanged(text: string)

    MouseArea {
        id: backgroundMouseArea
        anchors.fill: searchBar
        propagateComposedEvents: true // Allow clicks to pass through when needed
        onClicked: {
            searchField.focus = false;
        }
    }

    onFilterBy: (filter, search) => {
        console.log("Searching by " + filter + " and query: " + search)
        console.log("Current enabled status: " + modController.currentMod.enabled)
        modController.addFilter(filter, search)
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
                filter: searchBar.activeTag
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
            if (text === "by:" || text === "in:" && searchBar.isTagOpen) {
                console.log("Found tag: " + text + " in SearchBar.qml")
                searchBar.activeTag = text;
                searchBar.isTagOpen = true;

                if (text === "by:") {
                    searchBar.activeFilter = "authors";
                } else if (text === "in:") {
                    searchBar.activeFilter = "categories";
                }

                searchField.text = "";

                // wait for frame update
                Qt.callLater(() => {
                    searchField.leftPadding = activeFilterTag.width + 15;
                });
            }
        }

        onCleared: {
            searchBar.activeTag = ""
            searchBar.isTagOpen = false
            searchField.leftPadding = 8
            searchBar.searchTextChanged("");
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
                searchBar.activeTag = ""
                event.accepted = true
                searchField.leftPadding = 8
                // remove focus from search field
                searchField.focus = false
                // update signals
                searchBar.searchTextChanged("");
            }

            // Delete filter tag when backspace is pressed
            if (event.key === Qt.Key_Backspace && searchBar.isTagOpen && searchField.text === "") {
                console.log("Attempted to delete tag at SearchBar.qml with key event.")
                searchBar.isTagOpen = false
                searchBar.activeTag = ""
                searchField.leftPadding = 8            
            }

            // When user hits enter, search for the current text
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                console.log("Enter key hit at SearchBar.qml")
                searchField.focus = false
                searchBar.filterBy(searchBar.activeFilter, searchField.text)
            }
        }

        // MouseArea {
        //     anchors.fill: parent
        //     cursorShape: Qt.PointingHandCursor
        //     onClicked: console.log("Search clicked")
        // }
    }
}
