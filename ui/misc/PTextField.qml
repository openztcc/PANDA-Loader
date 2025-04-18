import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.labs.platform 1.1
import PandaUI 1.0

pragma ComponentBehavior: Bound

Item {
    id: pTextField
    Layout.preferredHeight: 90
    Layout.fillWidth: true

    property alias title: titleLabel.text
    property var bg: "#627D58"
    property var fg: "#ffffff"
    property var placeholderColor: "#E8E8CF"
    property var errorColor: "#FF0000"
    property var errorText: "Error"
    property var descriptionText: ""
    property var error: false
    property var filters: ["All files (*)"]
    property var mode: FileDialog.ExistingFiles
    property alias isFileBrowser: textField.isFileBrowser
    property alias leftPadding: textField.leftPadding
    property alias rightPadding: textField.rightPadding
    property alias topPadding: textField.topPadding
    property alias bottomPadding: textField.bottomPadding
    property alias placeholderText: textField.placeholderText
    property alias radius: textFieldBg.radius
    property alias trailingIcon: textFieldIcon.icon
    property alias border: textFieldBg.textFieldBorder
    property alias text: textField.text
    property var pHeight: null
    property string originalText: textField.text
    property bool focused: false

    // signals
    signal searchTextChanged(text: string)
    signal textChange(text: string)
    signal pressed(event: var)
    signal cleared()
    signal dirty(bool isDirty)

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
        anchors.fill: pTextField
        propagateComposedEvents: true
        onClicked: {
            textField.focus = false;
        }
    }

    Column {
        id: textFieldContainer
        spacing: 0
        anchors.fill: parent

        Label {
            id: titleLabel
            color: pTextField.placeholderColor
            font.pixelSize: 12
            anchors.left: textField.left
            visible: if (pTextField.title) {
                return true
            } else {
                return false
            }
            height: visible ? implicitHeight : 0
        }

        TextField {
            id: textField
            height: pTextField.pHeight ? pTextField.pHeight : 35
            anchors.top: if (pTextField.title) {
                titleLabel.bottom
            } else {
                parent.top
            }
            anchors.topMargin: if (pTextField.title) {
                5
            } else {
                0
            }
            anchors.left: parent.left
            anchors.right: parent.right
            readOnly: false
            leftPadding: 8
            color: pTextField.fg

            property var isFileBrowser: false
            property bool textFieldHovered: false

            background: Rectangle {
                id: textFieldBg
                property var textFieldBorder: 1
                color: textField.textFieldHovered ? Qt.darker(pTextField.bg, 1.05) : pTextField.bg
                radius: 5
                border.width: textFieldBorder
                border.color: pTextField.error ? pTextField.errorColor : Qt.darker(pTextField.bg, 1.3)
            }

            // change cursor color
            cursorDelegate: Rectangle {
                    id: cursor
                    visible: false
                    color: "#FED286"
                    width: 2

                    // add animation back since t his overrides default b ehavior
                    SequentialAnimation {
                        loops: Animation.Infinite
                        running: textField.cursorVisible

                        PropertyAction {
                            target: cursor
                            property: 'visible'
                            value: true
                        }

                        PauseAnimation {
                            duration: 600
                        }

                        PropertyAction {
                            target: cursor
                            property: 'visible'
                            value: false
                        }

                        PauseAnimation {
                            duration: 600
                        }

                        onStopped: {
                            cursor.visible = false
                        }
                    }
                }

            ClearButton {
                id: clearButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: {
                    if (pTextField.trailingIcon) {
                        textFieldIcon.left
                    } else if (textField.isFileBrowser) {
                        browseFilesButton.left
                    } else {
                        textField.right
                    }
                }
                anchors.rightMargin: 1
                width: pTextField.focused ? 35 : 0
                height: textField.height - 7
                textField: textField
                visible: pTextField.focused ? true : false
                fg: pTextField.placeholderColor
                bg: pTextField.bg
                z: 1

                onClicked: {
                    pTextField.cleared()
                }
            }

            SvgIcon {
                z: 5
                id: textFieldIcon
                anchors.right: textField.isFileBrowser ? browseFilesButton.left : textField.right
                anchors.rightMargin: 5
                height: textField.height
                visible: icon ? true : false
                anchors.verticalCenter: parent.verticalCenter
                iconWidth: 20
                iconHeight: 20
                color: "#ffffff"
                width: icon ? 35 : 0
                icon: ""
            }

            BrowseButton {
                id: browseFilesButton
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: textField.right
                anchors.rightMargin: 0
                height: textField.height - 7
                textField: textField
                fg: pTextField.placeholderColor
                bg: pTextField.bg
                fileDialog: fileDialogComponent
                component: pTextField
                z: 1
                width: if (textField.isFileBrowser) {
                    35
                } else {
                    0
                }
            }

            onFocusChanged: {
                if (focus) {
                    pTextField.focused = true
                } else {
                    pTextField.focused = false
                }
            }

            // Key handling
            Keys.onPressed: function(event) {
                // forward key events to the parent
                pTextField.pressed(event)
                // Allow Escape key to clear textfield
                if (event.key === Qt.Key_Escape) {
                    textField.text = ""
                    event.accepted = true
                    // remove focus from search field
                    textField.focus = false
                }
            }

            onTextChanged: {
                pTextField.textChange(textField.text)

                // // check if text is dirty
                // if (textField.text !== pTextField.originalText) {
                //     pTextField.dirty(true)
                // } else {
                //     pTextField.dirty(false)
                // }
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
            text: pTextField.descriptionText
            visible: text.trim().length > 0
            color: pTextField.placeholderColor
            font.pixelSize: 10
            anchors.top: textField.bottom
            anchors.topMargin: visible ? 5 : 0
            anchors.left: parent.left
            anchors.leftMargin: visible ? 10 : 0

            height: visible ? implicitHeight : 0
        }



        Label {
            id: errorLabel
            text: pTextField.errorText
            color: "red"
            font.pixelSize: 12
            visible: false
            anchors.top: descriptionLabel.bottom
            anchors.topMargin: visible ? 5 : 0
            anchors.left: parent.left
            anchors.leftMargin: visible ? 10 : 0

            height: visible ? implicitHeight : 0
        }

    }

}
