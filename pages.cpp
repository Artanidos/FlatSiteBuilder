/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "pages.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTest>

Pages::Pages()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *layout = new QGridLayout();
    QLabel *title = new QLabel();
    QPushButton *button = new QPushButton();
    button->setText("Add Page");
    title->setText("Pages");
    QTableWidget *list = new QTableWidget(3, 3, this);
    list->verticalHeader()->hide();
    QStringList labels;
    labels << "Name" << "Author" << "Actions";
    list->setHorizontalHeaderLabels(labels);
    list->setItem(0, 0, new QTableWidgetItem("Home"));
    list->setItem(0, 1, new QTableWidgetItem("Olaf Japp"));
    list->setItem(1, 0, new QTableWidgetItem("About"));
    list->setItem(1, 1, new QTableWidgetItem("Olaf Japp"));
    list->setItem(2, 0, new QTableWidgetItem("News"));
    list->setItem(2, 1, new QTableWidgetItem("Olaf Japp"));
    layout->addWidget(title, 0, 0);
    layout->addWidget(button, 1, 0);
    layout->addWidget(list, 2, 0, 1, 3);
    vbox->addLayout(layout);
    setLayout(vbox);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
}

void Pages::buttonClicked()
{
    emit addPage();
}
