import QtQuick 
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: browse

    property var fileDialog: null
    property var textField: null
    property var fg: textField.fg
    property var bg: ""
    property var component: null

    width: 55
    height: textField.height - 7

    Button {
        id: browseFilesButton
        property var fg: browse.fg
        property var bg: browse.bg 

        text: "Open"
        z: 1
        height: browse.textField.height
        width: parent.width
        flat: true
        background: Rectangle {
            id: browseFilesBg
            property var bg: browseFilesButton.bg
            property bool hovered: false
            color: hovered ? Qt.darker(bg, 1.5) : Qt.darker(bg, 1.2)
            anchors.fill: parent
            topRightRadius: 5
            bottomRightRadius: 5
            border.width: 1
            border.color: Qt.darker(browseFilesBg.bg, 1.6)
        }
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: browse.textField.right
        visible: browse.textField.isFileBrowser
        contentItem: SvgIcon {
            id: browseFilesIcon
            icon: "qrc:/icons/folder.svg"
            color: browse.fg
            iconWidth: 20
            iconHeight: 20
        }
        onClicked: {
            // Open file dialog here
            // TODO: update dialog to support multiple files
            console.log("Open file dialog")

            var dialog = browse.fileDialog.createObject(browse.component);
            // set properties
            dialog.fileMode = browse.component.mode;
            dialog.nameFilters = browse.component.filters;
            if (dialog) {
                dialog.open();
            } else {
                console.error("Failed to create FileDialog");
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton
        cursorShape: Qt.PointingHandCursor
        propagateComposedEvents: true
        onEntered: {
            browseFilesBg.hovered = true
        }
        onExited: {
            browseFilesBg.hovered = false
        }
        z: 1
    }
}
