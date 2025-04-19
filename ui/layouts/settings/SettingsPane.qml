import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

    Flickable {
        id: flick
        property string title: ""
        property color textColor: "white"
        property real spacing: 10
        default property alias content: contentContainer.children
        anchors.fill: parent
        width: parent.width
        contentWidth: width
        contentHeight: contentContainer.implicitHeight
        clip: false
        boundsBehavior: Flickable.StopAtBounds
        Layout.topMargin: 6
        Layout.bottomMargin: 6
        
        ColumnLayout {
            id: contentContainer
            width: flick.width
            spacing: flick.spacing

        }
    }

