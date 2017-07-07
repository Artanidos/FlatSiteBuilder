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

#include "contentlist.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTest>

ContentList::ContentList(Site *site, ContentType type)
{
    m_site = site;
    m_type = type;
    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *layout = new QGridLayout();
    QLabel *titleLabel = new QLabel();
    QPushButton *button = new QPushButton();
    button->setText(m_type == ContentType::Page ? "Add Page" : "Add Post");
    button->setMaximumWidth(120);
    titleLabel->setText(m_type == ContentType::Page ? "Pages" : "Posts");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    QTableWidget *list = new QTableWidget(0, 3, this);
    list->verticalHeader()->hide();
    list->setSelectionMode(QAbstractItemView::SingleSelection);
    list->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList labels;
    labels << "Name"  << "Layout" << "Author";
    list->setHorizontalHeaderLabels(labels);
    int rows = 0;
    for(int i = 0; i < site->contents().count(); i++)
    {
        Content *content = site->contents().at(i);
        if(content->contentType() == m_type)
        {
            list->setRowCount(rows + 1);
            QTableWidgetItem *titleItem = new QTableWidgetItem(content->title());
            titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
            list->setItem(rows, 0, titleItem);

            QTableWidgetItem *layoutItem = new QTableWidgetItem(content->layout());
            layoutItem->setFlags(layoutItem->flags() ^ Qt::ItemIsEditable);
            list->setItem(rows, 1, layoutItem);

            QTableWidgetItem *authorItem = new QTableWidgetItem(content->author());
            authorItem->setFlags(authorItem->flags() ^ Qt::ItemIsEditable);
            list->setItem(rows, 2, authorItem);
            rows++;
        }
    }

    layout->addWidget(titleLabel, 0, 0);
    layout->addWidget(button, 1, 0);
    layout->addWidget(list, 2, 0, 1, 3);
    vbox->addLayout(layout);
    setLayout(vbox);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
}

void ContentList::buttonClicked()
{
    emit addContent();
}

void ContentList::tableDoubleClicked(int row, int)
{
    emit editContent(m_site->contents().at(row));
}
