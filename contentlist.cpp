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
#include "tablecheckbox.h"
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
    m_deleteButton = new QPushButton();
    m_deleteButton->setText("Delete");
    m_deleteButton->setMaximumWidth(120);
    m_deleteButton->setEnabled(false);
    m_deleteButton->setToolTip("Delete all marked items");
    titleLabel->setText(m_type == ContentType::Page ? "Pages" : "Posts");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    m_list = new QTableWidget(0, 5, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name"  << "Layout" << "Author" << "Date";
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
    layout->addWidget(m_deleteButton, 3, 0);
    vbox->addLayout(layout);
    setLayout(vbox);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
    connect(m_deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
}

void ContentList::addListItem(Content *content)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCheckbox *checkBox = new TableCheckbox();
    connect(checkBox, SIGNAL(checkStateChanged(bool)), this, SLOT(checkStateChanged(bool)));
    m_list->setCellWidget(rows, 0, checkBox);
    m_list->setRowHeight(rows, checkBox->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(content->title());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(content));
    m_list->setItem(rows, 1, titleItem);

    QTableWidgetItem *layoutItem = new QTableWidgetItem(content->layout());
    layoutItem->setFlags(layoutItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 2, layoutItem);

    QTableWidgetItem *authorItem = new QTableWidgetItem(content->author());
    authorItem->setFlags(authorItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 3, authorItem);

    QTableWidgetItem *dateItem = new QTableWidgetItem(content->date().toString("dd.MM.yyyy"));
    dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 4, dateItem);
}

void ContentList::checkStateChanged(bool)
{
    int numberChecked = 0;
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        TableCheckbox *cb = dynamic_cast<TableCheckbox*>(m_list->cellWidget(row, 0));
        if(cb->checked() == Qt::Checked)
        {
            numberChecked++;
            break;
        }
    }
    m_deleteButton->setEnabled(numberChecked > 0);
}

void ContentList::buttonClicked()
{
    Content *content = new Content(m_type);
    m_site->addContent(content);
    addListItem(content);

    QTableWidgetItem *item = m_list->item(m_list->rowCount() - 1, 1);
    m_list->selectRow(m_list->rowCount() - 1);
    emit editContent(item);
}

void ContentList::deleteButtonClicked()
{
    QString filename;
    for(int row = m_list->rowCount() - 1; row >= 0; row--)
    {
        TableCheckbox *cb = dynamic_cast<TableCheckbox*>(m_list->cellWidget(row, 0));
        if(cb->checked() == Qt::Checked)
        {
            QTableWidgetItem *item = m_list->item(row, 1);
            Content *content = qvariant_cast<Content*>(item->data(Qt::UserRole));
            if(content->contentType() == ContentType::Page)
                filename = m_site->path() + "/pages/" + content->source();
            else
                filename = m_site->path() + "/posts/" + content->source();
            QFile file(filename);
            file.remove();

            m_site->removeContent(content);
            m_list->removeRow(row);
        }
    }
    m_deleteButton->setEnabled(false);
    emit contentUpdated("Delete Content");
}

void ContentList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    emit editContent(item);
}
