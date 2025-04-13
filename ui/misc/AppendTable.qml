import QtQuick
import QtQuick.Layouts

// this table is used to display and edit path variables

TableView {
    width: 600
    height: 300
    clip: true

    model: ListModel {
        ListElement { key: "PATH"; value: "/test/animal/building" }
        ListElement { key: "PATH"; value: "/test/animal/building" }
        ListElement { key: "PATH"; value: "/test/animal/building" }
        ListElement { key: "PATH"; value: "/test/animal/building" }
    }

    columnSpacing: 1
    rowSpacing: 1
    boundsBehavior: Flickable.StopAtBounds

    delegate: RowLayout {
        id: tRow
        required property string key
        required property string value

        spacing: 10
        Rectangle {
            implicitWidth: 200
            implicitHeight: 30
            color: "#333"
            Text {
                anchors.centerIn: parent
                text: tRow.key
                color: "white"
            }
        }
        Rectangle {
            implicitWidth: 400
            implicitHeight: 30
            color: "#444"
            Text {
                anchors.centerIn: parent
                text: tRow.value
                color: "white"
            }
        }
    }
}

