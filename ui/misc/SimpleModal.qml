import QtQuick 2.15
import QtQuick.Controls 6.5

Item {
    id: simpleModal
    property var acceptAction: null
    property var rejectAction: null
    property alias title: confirmationDialog.title
    property alias message: confirmMsg.text
    property alias centerTo: simpleModal.parent
    property bool showCancel: true

    Dialog {
        id: confirmationDialog

        title: ""
        modal: true
        standardButtons: Dialog.Yes | Dialog.No | Dialog.Cancel
        x: (simpleModal.centerTo.width - width) / 2
        y: (simpleModal.centerTo.height - height) / 2
        contentItem: Column {
            spacing: 10
            Label {
                id: confirmMsg
                text: "Placeholder text"
                wrapMode: Text.Wrap
            }
        }
        onAccepted: {
            if (simpleModal.action) {
                simpleModal.action()
            } else {
                console.log("No action defined")
            }
        }
        onRejected: {
            console.log(confirmationDialog.title + " cancelled")
            if (simpleModal.rejectAction) {
                simpleModal.rejectAction()
            } else {
                console.log("No reject action defined")
            }
        }
    }

    function open() {
        confirmationDialog.open()
    }
    function close() {
        confirmationDialog.close()
    }


}
