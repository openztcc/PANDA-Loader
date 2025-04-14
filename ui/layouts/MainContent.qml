import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

// generic content area for the main application

Pane {
    id: mainContent
    property var offset: null
    default property alias content: contentContainer.children
    anchors.fill: parent
    width: parent.width - 70
    height: parent.height - 40

    ColumnLayout {
        id: contentContainer
        Layout.fillHeight: true // remaining height
        Layout.fillWidth: true
        anchors.fill: parent


    }

}
