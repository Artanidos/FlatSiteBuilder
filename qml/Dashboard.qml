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
        columns: 4
        Text
        {
            text: "Dashboard"
            font.pointSize: 20
            color: "#ffffff"
        }

        Button
        {
            text: "Add Page"
            Layout.margins: 10
        }

        Button
        {
            text: "Add Post"
            Layout.margins: 10
        }

        Item
        {
            Layout.fillWidth: true
        }

        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 4
            clip: true
            color: "#404244"

            ListView
            {
                id: listview
                anchors.fill: parent
                model: PageModel {}
                delegate: delegateList
            }

            Component
            {
                id: delegateList
                Item
                {
                    id: delegateItem
                    width: listview.width
                    height: 40
                    clip: true

                    Column
                    {
                        id: column
                        x: 5
                        anchors
                        {
                            verticalCenter: parent.verticalCenter

                            margins: 10
                        }
                        Text
                        {
                            text: 'Name: ' + name
                            color: "#fff"
                        }
                    }

                    Column
                    {
                        id: column2
                        x: 200
                        anchors
                        {
                            verticalCenter: parent.verticalCenter
                            margins: 10
                        }

                        Text
                        {
                            text: 'Author: ' + author
                            color: "#fff"
                        }
                    }

                    Column
                    {
                        id: column3
                        x: 400
                        anchors
                        {
                            verticalCenter: parent.verticalCenter
                            left: parent.right
                            leftMargin: -140
                            margins: 10
                        }
                        Button { text: 'Edit'; height: 30}

                    }
                    Column
                    {
                        id: column4

                        anchors
                        {
                            verticalCenter: parent.verticalCenter
                            left: parent.right
                            leftMargin: -70
                            margins: 10
                        }

                        Button { text: 'Delete'; height: 30 }
                    }
                }
            }

        }
    }
}
