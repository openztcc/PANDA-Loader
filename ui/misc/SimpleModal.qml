import QtQuick
import QtQuick.Controls

Item {
    id: simpleModal
    property alias title: confirmationDialog.title
    property alias message: confirmMsg.text
    property alias centerTo: simpleModal.parent
    property bool showCancel: true

    signal cancelled()
    signal saved()
    signal discarded()


    Dialog {
        id: confirmationDialog

        title: ""
        modal: true
        x: (simpleModal.centerTo.width - width) / 2
        y: (simpleModal.centerTo.height - height) / 2
        contentItem: Column {
            spacing: 10
            Label {
                id: confirmMsg
                text: "Placeholder text"
                wrapMode: Text.Wrap
            }
            DialogButtonBox {
                id: buttons
                alignment: Qt.AlignRight
                spacing: 10

                Button {
                    text: "Save"
                    onClicked: {
                        simpleModal.saved()
                        confirmationDialog.close()
                    }
                }

                Button {
                    text: "Discard"
                    onClicked: {
                        simpleModal.discarded()
                        confirmationDialog.close()
                    }
                }

                Button {
                    visible: showCancel
                    text: "Cancel"
                    onClicked: {
                        simpleModal.cancelled()
                        confirmationDialog.close()
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


}
