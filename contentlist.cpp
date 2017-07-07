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
    m_list = new QTableWidget(0, 4, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch );
    QStringList labels;
    labels << "Name"  << "Layout" << "Author" << "Date";
    m_list->setHorizontalHeaderLabels(labels);
    int rows = 0;
    for(int i = 0; i < site->contents().count(); i++)
    {
        Content *content = site->contents().at(i);
        if(content->contentType() == m_type)
        {
            m_list->setRowCount(rows + 1);
            QTableWidgetItem *titleItem = new QTableWidgetItem(content->title());
            titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
            titleItem->setData(Qt::UserRole, QVariant::fromValue(content));
            m_list->setItem(rows, 0, titleItem);

            QTableWidgetItem *layoutItem = new QTableWidgetItem(content->layout());
            layoutItem->setFlags(layoutItem->flags() ^ Qt::ItemIsEditable);
            m_list->setItem(rows, 1, layoutItem);

            QTableWidgetItem *authorItem = new QTableWidgetItem(content->author());
            authorItem->setFlags(authorItem->flags() ^ Qt::ItemIsEditable);
            m_list->setItem(rows, 2, authorItem);

            QTableWidgetItem *dateItem = new QTableWidgetItem(content->date().toString("dd.MM.yyyy"));
            dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
            m_list->setItem(rows, 3, dateItem);
            rows++;
        }
    }

    layout->addWidget(titleLabel, 0, 0);
    layout->addWidget(button, 1, 0);
    layout->addWidget(m_list, 2, 0, 1, 3);
    vbox->addLayout(layout);
    setLayout(vbox);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
}

void ContentList::buttonClicked()
{
    emit addContent();
}

void ContentList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 0);
    emit editContent(qvariant_cast<Content*>(item->data(Qt::UserRole)));
}
