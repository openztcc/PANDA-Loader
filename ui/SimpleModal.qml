import QtQuick
import QtQuick.Effects


Item {
    id: simpleModal
    property var action
    property alias title: confirmationDialog.title
    property alias message: confirmMsg.text

    Dialog {
        id: confirmationDialog
        title: ""
        modal: true
        standardButtons: Dialog.Yes | Dialog.No
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        contentItem: Column {
            spacing: 10
            Label {
                id: confirmMsg
                text: "Placeholder text"
                wrapMode: Text.Wrap
            }
        }
        onAccepted: {
            if (action) {
                action()
            } else {
                console.log("No action defined")
            }
        }
        onRejected: {
            console.log(confirmationDialog.title + " cancelled")
        }
    }

    function open() {
        confirmationDialog.open()
    }
    function close() {
        confirmationDialog.close()
    }


}
