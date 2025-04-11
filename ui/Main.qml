import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import PandaLdr 1.0

pragma ComponentBehavior: Bound

ApplicationWindow {
    id: root
    width: 800
    height: 675
    visible: true
    title: qsTr("PANDA")

    Material.theme: Material.Light
    Material.accent: Material.LightGreen

    // Declare modals
    SimpleModal {
        id: confirmDialog
    }

    // Navigation Rail
    Drawer {
        id: navRail
        height: parent.height
        width: 70
        edge: Qt.LeftEdge
        modal: false  // Keep it always open
        interactive: false // prevents from closing when click away
        Component.onCompleted: navRail.open()

        contentItem: Navigation {
            id: navContent
        }
    }

    // Appbar
    ToolBar {
        id: toolbar
        Material.background: "#f7fbf2"
        width: parent.width - navRail.width
        anchors.right: parent.right
        
        AppBarLayout {
            id: appbar 
            anchors.fill: parent
            property var offset: navRail
        }
    }

    // Maint content
    Pane {
        id: mainContent
        width: parent.width - navRail.width
        height: parent.height - toolbar.height
        Material.background: "#9daa9e"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        padding: 6
        spacing: 12

        ModPage {
            id: modPage
        }
    }
}
