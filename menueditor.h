#ifndef MENUEDITOR_H
#define MENUEDITOR_H

#include "interfaces.h"

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

class MenuEditor : public AbstractEditor
{
    Q_OBJECT

public:
    MenuEditor(Menu *menu, Site *site);
    ~MenuEditor();

    void setContent(QString) {/* not used */}

signals:
    void contentUpdated(QString text);
    void contentChanged(Menu *menu);

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
};

#endif // MENUEDITOR_H
