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
#include "commands.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QTest>
#include <QComboBox>
#include <QUndoStack>
#include <QUndoCommand>

ContentList::ContentList(Site *site, ContentType type)
{
    m_site = site;
    m_addedContentName = "";
    m_type = type;
    m_undoStack = new QUndoStack;
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

    m_undo = new FlatButton(":/images/undo_normal.png", ":/images/undo_hover.png", "", ":/images/undo_disabled.png");
    m_redo = new FlatButton(":/images/redo_normal.png", ":/images/redo_hover.png", "", ":/images/redo_disabled.png");
    m_undo->setToolTip("Undo");
    m_redo->setToolTip("Redo");
    m_undo->setEnabled(false);
    m_redo->setEnabled(false);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch(0);
    hbox->addWidget(m_undo);
    hbox->addWidget(m_redo);

    m_list = new QTableWidget(0, 6, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name" << "Source" << "Layout" << "Author" << "Date";
    m_list->setHorizontalHeaderLabels(labels);

    reload();

    layout->addWidget(titleLabel, 0, 0);
    layout->addLayout(hbox, 0, 2);
    layout->addWidget(button, 1, 0);
    layout->addWidget(m_list, 2, 0, 1, 3);
    vbox->addLayout(layout);
    setLayout(vbox);

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
    connect(m_redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(m_undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SLOT(canUndoChanged(bool)));
    connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SLOT(canRedoChanged(bool)));
    connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
    connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SLOT(redoTextChanged(QString)));
}

ContentList::~ContentList()
{
    delete m_undoStack;
}

void ContentList::canUndoChanged(bool can)
{
    m_undo->setEnabled(can);
}

void ContentList::canRedoChanged(bool can)
{
    m_redo->setEnabled(can);
}

void ContentList::undoTextChanged(QString text)
{
    m_undo->setToolTip("Undo " + text);
}

void ContentList::redoTextChanged(QString text)
{
    m_redo->setToolTip("Redo " + text);
}

void ContentList::undo()
{
    m_undoStack->undo();
}

void ContentList::redo()
{
    m_undoStack->redo();
}

void ContentList::reload()
{
    m_list->setRowCount(0);

    Content *contentToEdit = NULL;
    int row = -1;

    if(m_type == ContentType::Page)
    {
        m_site->reloadPages();
        for(int i = 0; i < m_site->pages().count(); i++)
        {
            Content *content = m_site->pages().at(i);
            if(content->source() == m_addedContentName)
            {
                contentToEdit = content;
                row = m_list->rowCount();
            }
            addListItem(content);
        }
    }
    else
    {
        m_site->reloadPosts();
        for(int i = 0; i < m_site->posts().count(); i++)
        {
            Content *content = m_site->posts().at(i);
            if(content->source() == m_addedContentName)
            {
                contentToEdit = content;
                row = m_list->rowCount();
            }
            addListItem(content);
        }
    }
    if(contentToEdit != NULL)
    {
        m_addedContentName = "";
        m_list->selectRow(row);
        editContent(contentToEdit);
    }
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
    m_addedContentName = m_site->createTemporaryContent(m_type);
    QFileInfo info(m_addedContentName);
    m_addedContentName =  info.fileName();
    reload();
}

void ContentList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    m_undoStack->clear();
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
            QUndoCommand *delCommand = new DeleteContentCommand(this, m_site->sourcePath() + (m->contentType() == ContentType::Page ? "/pages/" : "/posts/") + m->source(), "delete content " + m->title());
            m_undoStack->push(delCommand);
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
            m_undoStack->clear();
            emit editContent(item);
            break;
        }
    }
}
