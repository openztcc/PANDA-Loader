import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Effects
import PandaUI 1.0

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
    Item {
        id: navRail
        width: 70
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        z: 1

        Navigation {
            id: navContent
            anchors.fill: parent
        }
    }

    // Appbar
    ToolBar {
        id: toolbar
        Material.background: "#f7fbf2"
        anchors.left: navRail.right
        anchors.right: parent.right
        anchors.top: parent.top
        height: 40

        AppBarLayout {
            id: appbar
            anchors.fill: parent
        }
    }

    StackView {
        id: stack
        anchors.top: toolbar.bottom
        anchors.left: navRail.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        initialItem: modPage
    }

    Component {
        id: modPage
        ModPage {
            anchors.left: navRail.left
            anchors.bottom: root.bottom
            width: root.width - 70
            height: root.height - 40

        }
    }

    Component {
        id: settingsPage
        SettingsPage { }
    }

    Component.onCompleted: {
        navRail.navContent.stack = stack
        navRail.navContent.modPage = modPage
        navRail.navContent.settingsPage = settingsPage
        navRail.open()
    }

}
