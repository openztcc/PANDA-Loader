import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: rField
    Layout.preferredHeight: 60
    Layout.fillWidth: true

    property alias title: titleLabel.text
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
            textField.focus = false;
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
            color: rField.placeholderColor
            font.pixelSize: 12
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        TextField {
            id: textField
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
                border.width: 1
                border.color: rField.error ? rField.errorColor : Qt.darker(rField.bg, 1.2)
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
                height: textField.height - 2 // to avoid overlap with border    
                width: 35
                flat: true
                background: Rectangle {
                    id: clearBg
                    property bool hovered: false
                    color: clearButton.hovered ? Qt.darker(rField.bg, 1.05) : rField.bg
                    anchors.fill: parent
                }
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: {
                    if (textField.isFileBrowser) {
                        browseFilesButton.left
                    } else {
                        textField.right
                    }
                }
                visible: textField.text.length > 0
                contentItem: SvgIcon {
                    id: clearIcon
                    icon: "qrc:/icons/close.svg"
                    color: rField.placeholderColor
                    iconWidth: 15
                    iconHeight: 15
                }
                onClicked: {
                    console.log("Clear clicked")
                    textField.text = ""
                    textField.focus = false
                }
            }

            Button {
                id: browseFilesButton
                text: "Open"
                height: textField.height
                width: 55
                flat: true
                background: Rectangle {
                    id: browseFilesBg
                    property bool hovered: false
                    color: browseFilesButton.hovered ? Qt.darker(rField.bg, 1.5) : Qt.darker(rField.bg, 1.2)
                    anchors.fill: parent
                    topRightRadius: 5
                    bottomRightRadius: 5

                    Rectangle {
                        height: 2
                        anchors.bottom: parent.bottom
                        width: parent.width
                        color: Qt.darker(rField.bg, 1.2)
                        anchors.rightMargin: 3
                    }
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: textField.right
                visible: textField.isFileBrowser
                contentItem: SvgIcon {
                    id: browseFilesIcon
                    icon: "qrc:/icons/folder.svg"
                    color: Qt.darker(rField.bg, 2.0)
                    iconWidth: 20
                    iconHeight: 20
                }
                onClicked: {
                    // Open file dialog here
                    console.log("Open file dialog")
                    // Implement file browsing logic here
                }
            }

            onFocusChanged: {
                // if (focus) {
                //     textField.placeholderText = ""
                // } else {
                //     if (textField.text == "") {
                //         textField.placeholderText = "Text"
                //     }
                // }
            }

            // Key handling
            Keys.onPressed: function(event) {
                // Allow Escape key to clear textfield
                if (event.key === Qt.Key_Escape) {
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
