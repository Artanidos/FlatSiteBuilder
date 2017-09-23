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

#ifndef MENUEDITOR_H
#define MENUEDITOR_H

#include "interfaces.h"
#include "animateableeditor.h"

class QUndoStack;
class FlatButton;
class QLineEdit;
class QPushButton;
class QTableWidget;
class QTreeWidget;
class QTreeWidgetItem;
class ImageSelector;
class MenuEditorTableCellButtons : public QWidget
{
    Q_OBJECT

public:
     MenuEditorTableCellButtons();

     void setMenuItem(MenuItem *m) {m_item = m;}
     void setEnableLeft(bool);
     void setEnableRight(bool);
     void setEnableUp(bool);
     void setEnableDown(bool);

private slots:
     void deleteItemClicked();
     void editItemClicked();
     void itemLeftClicked();
     void itemRightClicked();
     void itemUpClicked();
     void itemDownClicked();

signals:
     void deleteItem(MenuItem *);
     void editItem(MenuItem *);
     void itemLeft(MenuItem *);
     void itemRight(MenuItem *);
     void itemUp(MenuItem *);
     void itemDown(MenuItem *);

private:
     FlatButton *m_delete;
     FlatButton *m_edit;
     FlatButton *m_left;
     FlatButton *m_right;
     FlatButton *m_up;
     FlatButton *m_down;
     MenuItem *m_item;
};

class MenuEditor : public AnimateableEditor
{
    Q_OBJECT

public:
    MenuEditor(MainWindow *win, Menu *menu, Site *site);

    void setContent(QString) {/* not used */}
    void registerUndoStack(QUndoStack *stack);
    void reloadMenu(Menu *menu);

signals:
    void contentChanged(Menu *menu);
    void menuChanged(QString text);

private slots:
    void closeEditor();
    void nameChanged();
    void addButtonClicked();
    void itemChanged(QTreeWidgetItem*,int);
    void deleteItem(MenuItem *);
    void deleteSubItem(MenuItem *menuitem);
    void editItem(MenuItem *);
    void itemLeft(MenuItem *);
    void itemRight(MenuItem *);
    void itemUp(MenuItem *);
    void itemDown(MenuItem *);
    void canUndoChanged(bool can);
    void canRedoChanged(bool can);
    void undoTextChanged(QString text);
    void redoTextChanged(QString text);
    void undo();
    void redo();
    void iconClicked(ImageSelector*, Qt::MouseButton);

private:
    MainWindow *m_win;
    Menu *m_menu;
    FlatButton *m_close;
    FlatButton *m_undo;
    FlatButton *m_redo;
    QLineEdit *m_name;
    QTreeWidget *m_tree;
    QTreeWidgetItem *m_root;
    QUndoStack *m_undoStack;
    Site *m_site;

    void addTreeItem(MenuItem *item);
    int getRow(MenuItem *menuitem);
    void updateButtonStates();
    void resort();
    void addTableCellButtons(MenuItem *item, QTreeWidgetItem *twi);
    void menuUpdated(QString text);
    QString getUndoRedoText(MenuItem *item, QString action);
};

#endif // MENUEDITOR_H
