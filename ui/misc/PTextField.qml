import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: rField
    Layout.preferredHeight: 60
    Layout.fillWidth: true

    property string placeholderText: "Text"
    property var bg: "#627D58"
    property var fg: "#ffffff"
    property var placeholderColor: "#E8E8CF"
    property var errorColor: "#FF0000"
    property var errorText: "Error"
    property var descriptionText: "Description"
    property var error: false
    property alias isFileBrowser: textField.isFileBrowser

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

    Column {
        id: textFieldContainer
        spacing: 10
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5

        Label {
            id: titleLabel
            text: rField.placeholderText
            color: rField.placeholderColor
            font.pixelSize: 12
            anchors.left: parent.left
            anchors.leftMargin: 10
            visible: textField.text === ""
        }

        TextField {
            id: textField
            placeholderText: "Text"
            placeholderTextColor: rField.placeholderColor
            height: 35
            anchors.top: titleLabel.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.right: parent.right
            readOnly: false
            leftPadding: 8
            color: rField.fg

            property var isFileBrowser: false

            background: Rectangle {
                id: textFieldBg
                color: rField.bg
                radius: 5
            }

            Row {
                id: textContainer
                spacing: 5
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.rightMargin: 5

            }

            Button {
                id: clearButton
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: {
                    if (textField.isFileBrowser) {
                        browseFilesButton.left
                    } else {
                        textField.right
                    }
                }
                anchors.rightMargin: 10
                visible: textField.text !== ""
                contentItem: SvgIcon {
                    id: clearIcon
                    icon: "qrc:/icons/close.svg"
                    color: rField.placeholderColor
                    width: 20
                    height: 20
                }
                onClicked: {
                    rField.activeFilter = ""
                    textField.text = ""
                    rField.searchTextChanged(textField.text)
                    textField.focus = false
                }
            }

            Button {
                id: browseFilesButton
                text: "Open"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: textField.right
                anchors.rightMargin: 10
                visible: textField.isFileBrowser
                contentItem: SvgIcon {
                    id: browseFilesIcon
                    icon: "qrc:/icons/folder.svg"
                    color: rField.placeholderColor
                    width: 20
                    height: 20
                }
                onClicked: {
                    // Open file dialog here
                    console.log("Open file dialog")
                    // Implement file browsing logic here
                }
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

        Label {
            id: descriptionLabel
            text: rField.descriptionText
            color: rField.placeholderColor
            font.pixelSize: 10
            anchors.top: textField.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        Label {
            id: errorLabel
            text: rField.errorText
            color: "red"
            font.pixelSize: 12
            visible: false
            anchors.top: descriptionLabel.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

    }

}