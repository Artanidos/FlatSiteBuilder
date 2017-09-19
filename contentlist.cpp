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
#include "tablecellbuttons.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTest>
#include <QComboBox>

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
    m_list = new QTableWidget(0, 6, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name" << "Source" << "Layout" << "Author" << "Date";
    m_list->setHorizontalHeaderLabels(labels);

    if(m_site)
    {
        for(int i = 0; i < m_site->contents().count(); i++)
        {
            Content *content = m_site->contents().at(i);
            if(content->contentType() == m_type)
            {
                addListItem(content);
            }
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

void ContentList::addListItem(Content *content)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCellButtons *tcb = new TableCellButtons;
    tcb->setItem(content);
    connect(tcb, SIGNAL(deleteItem(QObject*)), this, SLOT(deleteContent(QObject*)));
    connect(tcb, SIGNAL(editItem(QObject*)), this, SLOT(editContent(QObject*)));
    m_list->setCellWidget(rows, 0, tcb);
    m_list->setRowHeight(rows, tcb->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(content->title());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(content));
    m_list->setItem(rows, 1, titleItem);

    QTableWidgetItem *sourceItem = new QTableWidgetItem(content->source());
    sourceItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 2, sourceItem);

    QTableWidgetItem *layoutItem = new QTableWidgetItem(content->layout());
    layoutItem->setFlags(layoutItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 3, layoutItem);

    QTableWidgetItem *authorItem = new QTableWidgetItem(content->author());
    authorItem->setFlags(authorItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 4, authorItem);

    QTableWidgetItem *dateItem = new QTableWidgetItem(content->date().toString("dd.MM.yyyy"));
    dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 5, dateItem);
}

void ContentList::buttonClicked()
{
    Content *content = new Content(m_type);
    if(m_type == ContentType::Page)
        content->setLayout("default");
    else
        content->setLayout("post");
    content->setAuthor(m_site->author());
    content->setKeywords(m_site->keywords());
    content->setMenu(m_site->menus().first()->name());
    m_site->addContent(content);
    addListItem(content);

    QTableWidgetItem *item = m_list->item(m_list->rowCount() - 1, 1);
    m_list->selectRow(m_list->rowCount() - 1);
    emit editContent(item);
}

void ContentList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    emit editContent(item);
}

void ContentList::deleteContent(QObject *content)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Content *m = qvariant_cast<Content*>(item->data(Qt::UserRole));
        if(m == content)
        {
            m_site->removeContent(m);
            m_list->removeRow(row);
            QFile file(m_site->sourcePath() + (m->contentType() == ContentType::Page ? "/pages/" : "/posts/") + m->source());
            file.remove();
            emit contentUpdated("Delete Content");
            break;
        }
    }
}

void ContentList::editContent(QObject *content)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Content *m = qvariant_cast<Content*>(item->data(Qt::UserRole));
        if(m == content)
        {
            m_list->selectRow(row);
            emit editContent(item);
            break;
        }
    }
}
