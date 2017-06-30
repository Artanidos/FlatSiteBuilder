#include "pages.h"
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPushButton>

Pages::Pages()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout();
    QLabel *pagesLabel = new QLabel("Pages");
    QFont font("Arial", 20);
    pagesLabel->setFont(font);
    QPushButton *addNew = new QPushButton();
    addNew->setText("Add Page");
    QListWidget *list = new QListWidget();
    hbox->addWidget(pagesLabel);
    hbox->addWidget(addNew);
    hbox->addStretch();
    vbox->addLayout(hbox);
    vbox->addWidget(list);
    setLayout(vbox);

    list->addItem("about.md");
    list->addItem("index.md");
    list->addItem("news.md");
}
