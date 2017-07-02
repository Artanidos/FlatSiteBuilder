import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtQuick.Layouts 1.3

Pane {
    Material.theme: Material.Dark
    Material.accent: Material.Teal

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Text {
                text: "Pages"
                font.pointSize: 20
                color: "#ffffff"
            }

            Button {
                text: "Add Page"
                Layout.margins: 10
            }
        }
        RowLayout {
            Text {
                text: "Name"
                color: "#fff"
                Layout.leftMargin: 5
                Layout.preferredWidth: 140
            }
            Text {
                text: "Author"
                color: "#fff"
                Layout.preferredWidth: 140
            }
            Text {
                text: "Url"
                color: "#fff"
                Layout.preferredWidth: 140
            }

            Text {
                text: "Layout"
                color: "#fff"
                Layout.preferredWidth: 140
            }
        }
        RowLayout {
            Rectangle {
                clip: true
                color: "#404244"
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: listview
                    anchors.fill: parent
                    model: site.pages
                    delegate: itemComponent
                    highlight: highlightComponent
                }

                Component {
                    id: highlightComponent

                    Rectangle {
                        width: ListView.view.width
                        color: "#45bbe6"
                    }
                }

                Component {
                    id: itemComponent

                    Item {
                        id: delegateItem
                        width: listview.width
                        height: 40
                        clip: true

                        MouseArea {
                            anchors.fill: parent
                            onClicked: listview.currentIndex = index
                        }
                        RowLayout {
                            Column {
                                id: column
                                Layout.preferredWidth: 140
                                Layout.leftMargin: 5
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    margins: 10
                                }
                                Text {
                                    text: title
                                    color: "#fff"
                                }
                            }

                            Column {
                                id: columnAuthor
                                Layout.preferredWidth: 140
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    margins: 10
                                }

                                Text {
                                    text: author
                                    color: "#fff"
                                }
                            }

                            Column {
                                id: columnUrl
                                Layout.preferredWidth: 140
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    margins: 10
                                }

                                Text {
                                    text: url
                                    color: "#fff"
                                }
                            }

                            Column {
                                id: columnLayout
                                Layout.preferredWidth: 140
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    margins: 10
                                }

                                Text {
                                    text: layout
                                    color: "#fff"
                                }
                            }

                            Column {
                                id: column3
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    margins: 10
                                }
                                Button { text: 'Edit'; height: 30}

                            }
                            Column {
                                id: column4
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    margins: 10
                                }

                                Button { text: 'Delete'; height: 30 }
                            }
                        }
                    }
                }
            }
        }
    }
}
