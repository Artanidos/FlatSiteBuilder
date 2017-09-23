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

#ifndef MENUELIST_H
#define MENUELIST_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include "site.h"
#include "undoableeditor.h"

class QUndoStack;
class MainWindow;
class MenuEditor;
class FlatButton;
class MenuList : public UndoableEditor
{
    Q_OBJECT

public:
    MenuList(MainWindow *win, Site *site);

    void load() override;
    void save() override;

    void registerMenuEditor(MenuEditor *editor);
    void unregisterMenuEditor();

private slots:
    void buttonClicked();
    void tableDoubleClicked(int, int);
    void deleteMenu(QObject *menu);
    void editMenu(QObject *menu);
    void menuChanged(QString text);

signals:
    void editContent(QTableWidgetItem *item);
    void editedItemChanged(QTableWidgetItem *item);

private:
    Site *m_site;
    QTableWidget *m_list;
    MainWindow *m_win;
    MenuEditor *m_editor;
    Menu *m_menuInEditor;

    QTableWidgetItem *addListItem(Menu *menu);
};

#endif // MENUELIST_H
