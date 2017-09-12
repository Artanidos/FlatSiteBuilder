#ifndef MENUEDITOR_H
#define MENUEDITOR_H

#include "interfaces.h"

class FlatButton;
class QLineEdit;
class QPushButton;
class QTableWidget;
class MenuEditor : public AbstractEditor
{
    Q_OBJECT

public:
    MenuEditor(Menu *menu);
    void setContent(QString) {/* not used */}

signals:
    void contentUpdated(QString text);
    void contentChanged(Menu *menu);

private slots:
    void closeEditor();
    void nameChanged();
    void buttonClicked();
    void deleteButtonClicked();
    void tableDoubleClicked(int, int);
    void checkStateChanged(bool);

private:
    Menu *m_menu;
    FlatButton *m_close;
    FlatButton *m_undo;
    FlatButton *m_redo;
    QLineEdit *m_name;
    QPushButton *m_deleteButton;
    QTableWidget *m_list;

    void addListItem(MenuItem *item);
};

#endif // MENUEDITOR_H
