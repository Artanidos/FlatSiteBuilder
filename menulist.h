#ifndef MENUELIST_H
#define MENUELIST_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include "site.h"

class QUndoStack;
class MainWindow;
class MenuEditor;
class FlatButton;
class MenuList : public QWidget
{
    Q_OBJECT

public:
    MenuList(MainWindow *win, Site *site);
    ~MenuList();

    void reloadMenu();
    void saveMenu();
    void registerMenuEditor(MenuEditor *editor);
    void unregisterMenuEditor();

private slots:
    void buttonClicked();
    void tableDoubleClicked(int, int);
    void deleteMenu(QObject *menu);
    void editMenu(QObject *menu);
    void menuChanged(QString text);
    void redo();
    void undo();
    void canUndoChanged(bool can);
    void canRedoChanged(bool can);
    void undoTextChanged(QString text);
    void redoTextChanged(QString text);

signals:
    void contentUpdated(QString text);
    void editContent(QTableWidgetItem *item);
    void editedItemChanged(QTableWidgetItem *item);

private:
    Site *m_site;
    QTableWidget *m_list;
    QUndoStack *m_undoStack;
    MainWindow *m_win;
    MenuEditor *m_editor;
    Menu *m_menuInEditor;
    FlatButton *m_undo;
    FlatButton *m_redo;

    QTableWidgetItem *addListItem(Menu *menu);
};

#endif // MENUELIST_H
