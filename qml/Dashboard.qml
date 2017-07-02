import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.1

Pane
{
    Material.theme: Material.Dark
    Material.accent: Material.Teal

    GridLayout
    {
        anchors.fill: parent
        columns: 3
        Text
        {
            text: "Dashboard"
            font.pointSize: 20
            color: "#ffffff"
            Layout.columnSpan: 2
        }

        Item
        {
            Layout.fillWidth: true
        }

        Text {
            text: "Title:"
            color: "#fff"
        }

        Text {
            text: "<b>" + site.title + "</b>"
            color: "#fff"
        }

        Text {}

        Text {
            text: "Theme:"
            color: "#fff"
        }

        Text {
            text: "<b>" + site.theme + "</b>"
            color: "#fff"
        }

        Text {}

        Item { Layout.fillHeight: true}

    }
}
