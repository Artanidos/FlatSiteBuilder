import QtQuick 2.0
import FlatSiteBuilder 1.0

Rectangle {
    property Site site
    signal close()

    color: "#353535"
    id: root

    Image {
        id: exit
        anchors.top: root.top
        anchors.right: root.right
        source: "images/close_normal.png"
        MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onPressed: root.close()
            onEntered: exit.source = "images/close_hover.png"
            onExited: exit.source = "images/close_normal.png"
        }
    }

    Image {
        anchors.centerIn: parent
        source: "images/image_placeholder.png"
        MouseArea {
            anchors.fill: parent
            //onPressed: seek.exec()
        }
    }
}
