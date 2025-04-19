import QtQuick
import QtQuick.Controls

Item {
    id: confirmDialogWrapper
    property alias title: confirmationDialog.title
    property alias message: confirmMsg.text
    property alias parentItem: confirmDialogWrapper.parent
    property bool showCancel: true

    signal confirmed(string result)

    Dialog {
        id: confirmationDialog
        modal: true
        width: 300
        x: (parentItem.width - width) / 2
        y: (parentItem.height - height) / 2

        contentItem: Column {
            spacing: 10
            padding: 20

            Label {
                id: confirmMsg
                text: "Are you sure?"
                wrapMode: Text.Wrap
                horizontalAlignment: Text.AlignHCenter
            }

            DialogButtonBox {
                alignment: Qt.AlignRight
                spacing: 10

                Button {
                    text: "Save"
                    onClicked: {
                        confirmationDialog.close()
                        confirmDialogWrapper.confirmed("save")
                    }
                }

                Button {
                    text: "Discard"
                    onClicked: {
                        confirmationDialog.close()
                        confirmDialogWrapper.confirmed("discard")
                    }
                }

                Button {
                    visible: showCancel
                    text: "Cancel"
                    onClicked: {
                        confirmationDialog.close()
                        confirmDialogWrapper.confirmed("cancel")
                    }
                }
            }
        }
    }

    function open() {
        confirmationDialog.open()
    }

    function close() {
        confirmationDialog.close()
    }

    // asynchronous modal handler for Promises
    function ask() {
        return new Promise(function(resolve) {
            open()

            function handler(result) {
                confirmDialogWrapper.confirmed.disconnect(handler)
                resolve(result)
            }

            confirmDialogWrapper.confirmed.connect(handler)
        })
    }
}
