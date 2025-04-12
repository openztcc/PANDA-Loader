import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.labs.platform 1.1

pragma ComponentBehavior: Bound

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
    property var filters: ["All files (*)"]
    property var mode: FileDialog.ExistingFiles
    property alias isFileBrowser: textField.isFileBrowser

    // signals
    signal searchTextChanged(text: string)

    Component {
        id: fileDialogComponent


        FileDialog {
            title: "Select a file"
            folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            nameFilters: ["All files (*)"]
            fileMode: FileDialog.ExistingFiles

            onAccepted: {
                if (file) {
                    textField.text = file.toString();
                }
            }

            onRejected: {
                console.log("File dialog canceled");
            }
        }
    }

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
            property bool textFieldHovered: false

            background: Rectangle {
                id: textFieldBg
                color: textField.textFieldHovered ? Qt.darker(rField.bg, 1.05) : rField.bg
                radius: 5
                border.width: 1
                border.color: rField.error ? rField.errorColor : Qt.darker(rField.bg, 1.2)

                // shadow effect
                Rectangle {
                    anchors.top: parent.top
                    color: Qt.darker(rField.bg, 1.18)
                    height: 4
                    width: {
                        if (textField.isFileBrowser) {
                            parent.width - browseFilesButton.width
                        } else {
                            parent.width
                        }
                    }
                    topLeftRadius: 5
                    topRightRadius: 5
                }
            }

            ClearButton {
                id: clearButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: {
                    if (textField.isFileBrowser) {
                        browseFilesButton.left
                    } else {
                        textField.right
                    }
                }
                anchors.rightMargin: 0
                width: 35
                height: textField.height - 7
                textField: textField
                fg: rField.placeholderColor 
                bg: rField.bg
                z: 1
            }

            Button {
                id: browseFilesButton
                text: "Open"
                z: 1
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
                    color: rField.placeholderColor
                    iconWidth: 20
                    iconHeight: 20
                }
                onClicked: {
                    // Open file dialog here
                    // TODO: update dialog to support multiple files
                    console.log("Open file dialog")

                    var dialog = fileDialogComponent.createObject(rField);
                    // set properties
                    dialog.fileMode = rField.mode;
                    dialog.nameFilters = rField.filters;
                    if (dialog) {
                        dialog.open();
                    } else {
                        console.error("Failed to create FileDialog");
                    }
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

            MouseArea {
                id: textFieldMouseArea
                anchors.fill: parent
                hoverEnabled: true 
                acceptedButtons: Qt.NoButton
                onEntered: {
                    textField.textFieldHovered = true
                }
                onExited: {
                    textField.textFieldHovered = false
                }
                cursorShape: Qt.IBeamCursor
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
