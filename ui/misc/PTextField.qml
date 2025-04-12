import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: rField
    Layout.preferredHeight: 40
    Layout.fillWidth: true

    property string placeholderText: "Text"

    // signals
    signal searchTextChanged(text: string)

    MouseArea {
        id: backgroundMouseArea
        anchors.fill: rField
        propagateComposedEvents: true
        onClicked: {
            searchField.focus = false;
        }
    }

    TextField {
        id: textField
        placeholderText: "Text"
        placeholderTextColor: "#424940"
        anchors.fill: parent
        readOnly: false
        leftPadding: 8

        background: Rectangle {
            id: textFieldBg
            color: "#f7fbf2"
            radius: 0
        }

        Row {
            id: textContainer
            spacing: 5
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5

        }

        onFocusChanged: {
            if (focus) {
                textField.placeholderText = ""
            } else {
                if (textField.text == "") {
                    textField.placeholderText = "Text"
                }
            }
        }

        // Key handling
        Keys.onPressed: function(event) {
            // Allow Escape key to clear textfield
            if (event.key === Qt.Key_Escape) {
                rField.activeFilter = ""
                textField.text = ""
                event.accepted = true
                // remove focus from search field
                textField.focus = false
            }
        }
    }
}