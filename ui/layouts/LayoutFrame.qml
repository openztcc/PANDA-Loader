import QtQuick 2.15
import QtQuick.Layouts 1.15

// generic content area for the main application

Item {
    id: mainContent
    property var offset: null
    default property alias content: contentContainer.children
    anchors.fill: parent

    ColumnLayout {
        id: contentContainer
        Layout.fillHeight: true // remaining height
        Layout.fillWidth: true
        anchors.fill: parent


    }

}
