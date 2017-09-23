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

#include "menueditor.h"
#include "flatbutton.h"
#include "imageselector.h"
#include "generator.h"
#include "commands.h"
#include <QUndoStack>
#include <QFileDialog>
#include <QStandardPaths>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QTreeWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QStatusBar>
#include <QTest>

MenuEditor::MenuEditor(MainWindow *win, Menu *menu, Site *site)
{
    m_win = win;
    m_site = site;
    m_changed = false;

    setAutoFillBackground(true);

    QLabel *titleLabel = new QLabel("Menu Editor");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    m_close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    m_close->setToolTip("Close Content Editor");
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
    hbox->addWidget(m_close);

    QPushButton *addButton = new QPushButton("Add Menuitem");
    addButton->setMaximumWidth(120);

    m_name = new QLineEdit;
    m_name->setText(menu->name());
    m_name->setMaximumWidth(200);

    QStringList labels;
    labels << "Title" << "Url" << "Icon" << "" << "Sort";

    m_tree = new QTreeWidget;
    m_tree->setHeaderLabels(labels);
    m_tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tree->header()->hideSection(4);
    m_tree->setSelectionMode(QAbstractItemView::NoSelection);
    m_tree->setToolTip("Double Click To Edit");
    m_tree->setColumnWidth(2, 40);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(titleLabel, 0, 0);
    layout->addLayout(hbox, 0, 2);
    layout->addWidget(new QLabel("Name"), 1, 0);
    layout->addWidget(m_name, 2, 0);
    layout->addWidget(addButton, 3, 0);
    layout->addWidget(m_tree, 4, 0, 1, 3);
    setLayout(layout);

    reloadMenu(menu);

    connect(addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    connect(m_close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_name, SIGNAL(editingFinished()), this, SLOT(nameChanged()));
    connect(m_redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(m_undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(m_tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChanged(QTreeWidgetItem*,int)));
}

void MenuEditor::reloadMenu(Menu *menu)
{
    if(menu == NULL)
    {
        emit close();
        return;
    }
    m_menu = menu;
    m_tree->clear();

    m_name->setText(menu->name());
    for(int i = 0; i < m_menu->items().count(); i++)
    {
        MenuItem *item = m_menu->items().at(i);
        addTreeItem(item);
    }
    m_tree->expandAll();
    m_tree->sortItems(4, Qt::AscendingOrder);
    updateButtonStates();
}

void MenuEditor::registerUndoStack(QUndoStack *stack)
{
    m_undoStack = stack;
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SLOT(canUndoChanged(bool)));
    connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SLOT(canRedoChanged(bool)));
    connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
    connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SLOT(redoTextChanged(QString)));
    m_undo->setEnabled(m_undoStack->canUndo());
    m_redo->setEnabled(m_undoStack->canRedo());
    m_undo->setToolTip("Undo " + m_undoStack->undoText());
    m_redo->setToolTip("Redo " + m_undoStack->redoText());
}

void MenuEditor::canUndoChanged(bool can)
{
    m_undo->setEnabled(can);
}

void MenuEditor::canRedoChanged(bool can)
{
    m_redo->setEnabled(can);
}

void MenuEditor::undoTextChanged(QString text)
{
    m_undo->setToolTip("Undo " + text);
}

void MenuEditor::redoTextChanged(QString text)
{
    m_redo->setToolTip("Redo " + text);
}

void MenuEditor::undo()
{
    m_undoStack->undo();
}

void MenuEditor::redo()
{
    m_undoStack->redo();
}

void MenuEditor::itemChanged(QTreeWidgetItem *twi, int column)
{
    QString action = "";
    MenuItem *item = qvariant_cast<MenuItem*>(twi->data(0, Qt::UserRole));
    switch(column)
    {
        case 0:
            item->setTitle(twi->text(0));
            action = "title changed";
            break;
        case 1:
            item->setUrl(twi->text(1));
            action = "url changed";
            break;
    }
    emit menuChanged(getUndoRedoText(item, action));
}

void MenuEditor::iconClicked(ImageSelector *is, Qt::MouseButton button)
{
    MenuItem *mi = dynamic_cast<MenuItem*>(is->item());
    QString action = "";
    if(button == Qt::LeftButton)
    {
        QString fileName;
        QFileDialog *dialog = new QFileDialog();
        dialog->setFileMode(QFileDialog::AnyFile);
        dialog->setNameFilter(tr("Images (*.png *.gif *.jpg);;All (*)"));
        dialog->setWindowTitle(tr("Load Image"));
        dialog->setOption(QFileDialog::DontUseNativeDialog, true);
        dialog->setAcceptMode(QFileDialog::AcceptOpen);
        if(dialog->exec())
            fileName = dialog->selectedFiles().first();
        delete dialog;
        if (fileName.isEmpty())
            return;

        // copy file to assets dir
        QFileInfo info(fileName);
        QString name = info.fileName().replace(" ", "_");
        QString path = m_site->sourcePath() + "/assets/images/" + name;
        QFile::copy(fileName, path);

        // also copy file to deploy dir for previews
        QString dpath = m_site->deployPath() + "/assets/images/" + name;
        QFile::copy(fileName, dpath);

        mi->setIcon(path.mid(path.indexOf("assets/images/")));
        is->setImage(QImage(path));
        action = "icon changed";
    }
    else if(button == Qt::RightButton)
    {
        action = "icon removed";
        mi->setIcon("");
        is->setImage(QImage(":/images/image_placeholder.png"));
    }
    emit menuChanged(getUndoRedoText(mi, action));
}

void MenuEditor::addTreeItem(MenuItem *item)
{
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setFlags(twi->flags() | Qt::ItemIsEditable);
    twi->setText(0, item->title());
    twi->setText(1, item->url());

    twi->setText(4, QString::number(m_tree->topLevelItemCount()));
    twi->setData(0, Qt::UserRole, QVariant::fromValue(item));
    m_tree->addTopLevelItem(twi);
    addTableCellButtons(item, twi);

    for(int i = 0; i < item->items().count(); i++)
    {
        MenuItem *sub = item->items().at(i);
        QTreeWidgetItem *stwi = new QTreeWidgetItem();
        stwi->setFlags(stwi->flags() | Qt::ItemIsEditable);
        stwi->setText(0, sub->title());
        stwi->setText(1, sub->url());
        stwi->setText(4, QString::number(i));
        stwi->setData(0, Qt::UserRole, QVariant::fromValue(sub));
        twi->addChild(stwi);
        addTableCellButtons(sub, stwi);
    }
}

void MenuEditor::addTableCellButtons(MenuItem *item, QTreeWidgetItem *twi)
{
    MenuEditorTableCellButtons *tcb = new MenuEditorTableCellButtons();
    tcb->setMenuItem(item);
    m_tree->setItemWidget(twi, 3, tcb);
    m_tree->setColumnWidth(3, tcb->sizeHint().width());
    if(item->isSubitem())
    {
        connect(tcb, SIGNAL(deleteItem(MenuItem*)), this, SLOT(deleteSubItem(MenuItem*)));
        connect(tcb, SIGNAL(itemLeft(MenuItem*)), this, SLOT(itemLeft(MenuItem*)));
    }
    else
    {
        connect(tcb, SIGNAL(deleteItem(MenuItem*)), this, SLOT(deleteItem(MenuItem*)));
        connect(tcb, SIGNAL(itemUp(MenuItem*)), this, SLOT(itemUp(MenuItem*)));
        connect(tcb, SIGNAL(itemDown(MenuItem*)), this, SLOT(itemDown(MenuItem*)));
        connect(tcb, SIGNAL(itemRight(MenuItem*)), this, SLOT(itemRight(MenuItem*)));
        connect(tcb, SIGNAL(editItem(MenuItem*)), this, SLOT(editItem(MenuItem*)));
    }

    ImageSelector *is = new ImageSelector;
    is->setToolTip("Click to select image, right click to reset image");
    is->setItem(item);
    is->setMaximumSize(24, 24);
    QWidget *isw = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(is);
    isw->setLayout(vbox);
    if(item->icon().isEmpty())
        is->setImage(QImage(":/images/image_placeholder.png"));
    else
        is->setImage(QImage(m_site->sourcePath() + "/" + item->icon()));
    m_tree->setItemWidget(twi, 2, isw);
    connect(is, SIGNAL(clicked(ImageSelector*,Qt::MouseButton)), this, SLOT(iconClicked(ImageSelector*,Qt::MouseButton)));
}

void MenuEditor::closeEditor()
{
    emit close();
}

void MenuEditor::nameChanged()
{
    if(m_menu->name() != m_name->text())
    {
        QString action = "menu name changed from \"" + m_menu->name() + "\" to \"" + m_name->text() + "\"";
        m_menu->setName(m_name->text());
        emit contentChanged(m_menu);
        emit menuChanged(action);
    }
}

void MenuEditor::addButtonClicked()
{
    MenuItem *menuitem = new MenuItem();
    m_menu->addMenuitem(menuitem);
    addTreeItem(menuitem);
    emit menuChanged(getUndoRedoText(menuitem, "added"));

    updateButtonStates();
    QTreeWidgetItem *item = m_tree->topLevelItem(getRow(menuitem));
    m_tree->editItem(item, 0);
}

void MenuEditor::deleteItem(MenuItem *menuitem)
{
    m_menu->removeItem(menuitem);
    QTreeWidgetItem *item = m_tree->takeTopLevelItem(getRow(menuitem));
    delete item;
    updateButtonStates();
    emit menuChanged(getUndoRedoText(menuitem, "deleted"));
    delete menuitem;
}

void MenuEditor::deleteSubItem(MenuItem *menuitem)
{
    int row = getRow(menuitem->parentItem());
    QTreeWidgetItem *parent = m_tree->topLevelItem(row);
    for(int i = 0; i < parent->childCount(); i++)
    {
        QTreeWidgetItem *child = parent->child(i);
        MenuItem *mi = qvariant_cast<MenuItem*>(child->data(0, Qt::UserRole));
        if(mi == menuitem)
        {
            parent->removeChild(child);
            delete child;
            break;
        }
    }
    menuitem->parentItem()->removeMenuitem(menuitem);
    delete menuitem;
    updateButtonStates();
    emit menuChanged(getUndoRedoText(menuitem, "deleted"));
}

void MenuEditor::editItem(MenuItem *menuitem)
{
    QTreeWidgetItem *item = m_tree->topLevelItem(getRow(menuitem));
    m_tree->editItem(item, 0);
}

void MenuEditor::itemLeft(MenuItem *menuitem)
{
    int row = getRow(menuitem->parentItem());
    QTreeWidgetItem *parent = m_tree->topLevelItem(row);
    for(int i = 0; i < parent->childCount(); i++)
    {
        QTreeWidgetItem *child = parent->child(i);
        MenuItem *mi = qvariant_cast<MenuItem*>(child->data(0, Qt::UserRole));
        if(mi == menuitem)
        {
            menuitem->parentItem()->removeMenuitem(menuitem);
            menuitem->setSubitem(false);
            m_menu->addMenuitem(menuitem);
            parent->takeChild(i);
            addTreeItem(menuitem);
            break;
        }
    }

    updateButtonStates();
    emit menuChanged(getUndoRedoText(menuitem, "changed to top item"));
}

void MenuEditor::itemRight(MenuItem *menuitem)
{
    int row = getRow(menuitem);
    QTreeWidgetItem *parent = m_tree->topLevelItem(row - 1);
    QTreeWidgetItem *item = m_tree->takeTopLevelItem(row);
    parent->addChild(item);
    m_menu->removeItem(menuitem);
    MenuItem *parentItem = qvariant_cast<MenuItem*>(parent->data(0, Qt::UserRole));
    parentItem->addMenuitem(menuitem);
    menuitem->setSubitem(true);
    addTableCellButtons(menuitem, item);
    m_tree->expandAll();
    updateButtonStates();
    emit menuChanged(getUndoRedoText(menuitem, "changed to sub item"));
}

void MenuEditor::itemUp(MenuItem *menuitem)
{
    int row = getRow(menuitem);
    QTreeWidgetItem *sortItem1 = m_tree->topLevelItem(row - 1);
    QTreeWidgetItem *sortItem2 = m_tree->topLevelItem(row);
    sortItem1->setText(4, QString::number(row));
    sortItem2->setText(4, QString::number(row - 1));
    m_tree->sortItems(4, Qt::AscendingOrder);
    resort();
    updateButtonStates();
    emit menuChanged(getUndoRedoText(menuitem, "sorted up"));
}

void MenuEditor::itemDown(MenuItem *menuitem)
{
    int row = getRow(menuitem);
    QTreeWidgetItem *sortItem1 = m_tree->topLevelItem(row);
    QTreeWidgetItem *sortItem2 = m_tree->topLevelItem(row + 1);
    sortItem1->setText(4, QString::number(row + 1));
    sortItem2->setText(4, QString::number(row));
    m_tree->sortItems(4, Qt::AscendingOrder);
    resort();
    updateButtonStates();
    emit menuChanged(getUndoRedoText(menuitem, "sorted down"));
}

QString MenuEditor::getUndoRedoText(MenuItem *item, QString action)
{
    return "menuitem (" + item->title() + ") from menu (" + m_menu->name() + ") " + action;
}

int MenuEditor::getRow(MenuItem *menuitem)
{
    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = m_tree->topLevelItem(i);
        MenuItem *mi = qvariant_cast<MenuItem*>(item->data(0, Qt::UserRole));
        if(mi == menuitem)
            return i;
    }
    return -1;
}

void MenuEditor::updateButtonStates()
{
    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *twi = m_tree->topLevelItem(i);
        MenuEditorTableCellButtons *tcb = dynamic_cast<MenuEditorTableCellButtons*>(m_tree->itemWidget(twi, 3));
        MenuItem *menuitem = qvariant_cast<MenuItem*>(twi->data(0, Qt::UserRole));
        tcb->setEnableDown(i != m_tree->topLevelItemCount() - 1);
        tcb->setEnableUp(i != 0);
        tcb->setEnableRight(i != 0 && menuitem->items().count() == 0);
        tcb->setEnableLeft(false);
        for(int j = 0; j < twi->childCount(); j++)
        {
            QTreeWidgetItem *stwi = twi->child(j);
            MenuEditorTableCellButtons *stcb = dynamic_cast<MenuEditorTableCellButtons*>(m_tree->itemWidget(stwi, 3));
            stcb->setEnableLeft(true);
        }
    }
}

void MenuEditor::resort()
{
    while(m_menu->items().count() > 0)
        m_menu->removeItem(m_menu->items().at(0));

    for(int i = 0; i < m_tree->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = m_tree->topLevelItem(i);
        MenuItem *mi = qvariant_cast<MenuItem*>(item->data(0, Qt::UserRole));
        m_menu->addMenuitem(mi);
    }
}

MenuEditorTableCellButtons::MenuEditorTableCellButtons()
{
    m_delete = new FlatButton(":/images/trash_normal.png", ":/images/trash_hover.png");
    m_edit = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_left = new FlatButton(":/images/left_normal.png", ":/images/left_hover.png", "", ":/images/left_disabled.png");
    m_right = new FlatButton(":/images/right_normal.png", ":/images/right_hover.png", "", ":/images/right_disabled.png");
    m_up = new FlatButton(":/images/up_normal.png", ":/images/up_hover.png", "", ":/images/up_disabled.png");
    m_down = new FlatButton(":/images/down_normal.png", ":/images/down_hover.png", "", ":/images/down_disabled.png");
    m_edit->setToolTip("Edit Item");
    m_delete->setToolTip("Delete Item");
    m_left->setToolTip("Make Mainitem");
    m_right->setToolTip("Make Subitem");
    m_up->setToolTip("Sort Up");
    m_down->setToolTip("Sort Down");
    m_left->setEnabled(false);
    m_right->setEnabled(false);
    m_up->setEnabled(false);
    m_down->setEnabled(false);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_edit);
    hbox->addWidget(m_up);
    hbox->addWidget(m_down);
    hbox->addWidget(m_left);
    hbox->addWidget(m_right);
    hbox->addWidget(m_delete);
    setLayout(hbox);

    connect(m_delete, SIGNAL(clicked()), this, SLOT(deleteItemClicked()));
    connect(m_edit, SIGNAL(clicked()), this, SLOT(editItemClicked()));
    connect(m_left, SIGNAL(clicked()), this, SLOT(itemLeftClicked()));
    connect(m_right, SIGNAL(clicked()), this, SLOT(itemRightClicked()));
    connect(m_up, SIGNAL(clicked()), this, SLOT(itemUpClicked()));
    connect(m_down, SIGNAL(clicked()), this, SLOT(itemDownClicked()));
}

void MenuEditorTableCellButtons::deleteItemClicked()
{
    emit deleteItem(m_item);
}

void MenuEditorTableCellButtons::editItemClicked()
{
    emit editItem(m_item);
}

void MenuEditorTableCellButtons::itemLeftClicked()
{
    emit itemLeft(m_item);
}

void MenuEditorTableCellButtons::itemRightClicked()
{
    emit itemRight(m_item);
}

void MenuEditorTableCellButtons::itemUpClicked()
{
    emit itemUp(m_item);
}

void MenuEditorTableCellButtons::itemDownClicked()
{
    emit itemDown(m_item);
}

void MenuEditorTableCellButtons::setEnableLeft(bool mode)
{
    m_left->setEnabled(mode);
}

void MenuEditorTableCellButtons::setEnableRight(bool mode)
{
    m_right->setEnabled(mode);
}

void MenuEditorTableCellButtons::setEnableUp(bool mode)
{
    m_up->setEnabled(mode);
}

void MenuEditorTableCellButtons::setEnableDown(bool mode)
{
    m_down->setEnabled(mode);
}
