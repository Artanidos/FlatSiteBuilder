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
            text: "Pages"
            font.pointSize: 20
            color: "#ffffff"
        }

        Button
        {
            text: "Add Page"
            Layout.margins: 10
        }

        Item
        {
            Layout.fillWidth: true
        }

        Text
        {
            text: "Name"
            color: "#fff"
        }
        Text
        {
            id: authorHeader
            text: "Author"
            color: "#fff"
        }
        Item{}
        Item {}
        Rectangle
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.columnSpan: 3
            clip: true
            color: "#404244"

            ListView
            {
                id: listview
                anchors.fill: parent
                model: site.pages
                delegate: itemComponent
                highlight: highlightComponent
            }

            Component
            {
                id: highlightComponent

                Rectangle
                {
                    width: ListView.view.width
                    color: "#45bbe6"
                }
            }

            Component
            {
                id: itemComponent

                Item
                {
                    id: delegateItem
                    width: listview.width
                    height: 40
                    clip: true

                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: listview.currentIndex = index
                    }

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
                            text: title
                            color: "#fff"
                        }
                    }

                    Column
                    {
                        id: column2
                        x: 140
                        anchors
                        {
                            verticalCenter: parent.verticalCenter
                            margins: 10
                        }

                        Text
                        {
                            text: author
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
