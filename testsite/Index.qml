import FlatSiteBuilder 1.0

Page {
    layout: "default"
	title: "Home"
    url: "index.html"
    author: "Olaf Japp"

    Section {
        Row {
            Column {
                Text {
                    text: "<h1>Welcome</h1>"
                }
            }
        }
    }

    Section {
        Row {
            Column {
                Text {
                    text: "<p>How are you</p>"
                }
            }
        }
    }
}
