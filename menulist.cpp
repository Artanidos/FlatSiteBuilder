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

#include "menulist.h"
#include "mainwindow.h"
#include "menueditor.h"
#include "commands.h"
#include "flatbutton.h"
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include <QUndoCommand>
#include <QUndoStack>
#include <QXmlStreamWriter>
#include <QTest>
#include "flatbutton.h"
#include "tablecellbuttons.h"

MenuList::MenuList(MainWindow *win, Site *site)
{
    m_win = win;
    m_site = site;
    m_menuInEditor = NULL;
    m_editor = NULL;
    m_titleLabel->setText("Menus");
    m_filename = site->sourcePath() + "/Menus.xml";
    QPushButton *button = new QPushButton("Add Menu");
    button->setMaximumWidth(120);

    m_list = new QTableWidget(0, 2, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name";
    m_list->setHorizontalHeaderLabels(labels);

    m_layout->addWidget(button, 1, 0);
    m_layout->addWidget(m_list, 2, 0, 1, 3);

    load();

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
}

void MenuList::registerMenuEditor(MenuEditor *editor)
{
    m_editor = editor;
    m_editor->registerUndoStack(m_undoStack);
    connect(m_editor, SIGNAL(menuChanged(QString)), this, SLOT(menuChanged(QString)));
}

void MenuList::unregisterMenuEditor()
{
    m_editor = NULL;
}

void MenuList::load()
{
    m_list->clearContents();
    m_list->setRowCount(0);
    m_site->loadMenus();

    int menuId = -1;
    if(m_menuInEditor)
        menuId = m_menuInEditor->id();
    m_menuInEditor = NULL;
    foreach(Menu *menu, m_site->menus())
    {
        QTableWidgetItem *item = addListItem(menu);
        if(menu->id() == menuId)
        {
            m_list->selectRow(m_list->rowCount() - 1);
            m_menuInEditor = menu;
            emit editedItemChanged(item);
        }
    }

    if(m_editor)
        m_editor->reloadMenu(m_menuInEditor);
}

void MenuList::save()
{
    m_site->saveMenus();
}

void MenuList::menuChanged(QString text)
{
    contentChanged(text);
}

QTableWidgetItem *MenuList::addListItem(Menu *menu)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCellButtons *tcb = new TableCellButtons;
    tcb->setItem(menu);
    connect(tcb, SIGNAL(deleteItem(QObject*)), this, SLOT(deleteMenu(QObject*)));
    connect(tcb, SIGNAL(editItem(QObject*)), this, SLOT(editMenu(QObject*)));
    m_list->setCellWidget(rows, 0, tcb);
    m_list->setRowHeight(rows, tcb->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(menu->name());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(menu));
    m_list->setItem(rows, 1, titleItem);
    return titleItem;
}

void MenuList::buttonClicked()
{
    Menu *menu = new Menu();
    m_site->addMenu(menu);
    addListItem(menu);
    m_list->selectRow(m_list->rowCount() - 1);
    menuChanged("menu added");
    tableDoubleClicked(m_list->rowCount() - 1, 0);
}

void MenuList::deleteMenu(QObject *menu)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Menu *m = qvariant_cast<Menu*>(item->data(Qt::UserRole));
        if(m == menu)
        {
            m_site->removeMenu(m);
            m_list->removeRow(row);
            menuChanged("menu \"" + m->name() + "\" deleted");
            break;
        }
    }
}

void MenuList::editMenu(QObject *menu)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Menu *m = qvariant_cast<Menu*>(item->data(Qt::UserRole));
        if(m == menu)
        {
            m_menuInEditor = m;
            m_list->selectRow(row);
            emit editContent(item);
            break;
        }
    }
}

void MenuList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    m_menuInEditor = qvariant_cast<Menu*>(item->data(Qt::UserRole));
    emit editContent(item);
}
