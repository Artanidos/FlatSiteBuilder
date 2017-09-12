#ifndef MENUELIST_H
#define MENUELIST_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include "site.h"

class MenuList : public QWidget
{
    Q_OBJECT

public:
    MenuList(Site *site);

private slots:
    void buttonClicked();
    void deleteButtonClicked();
    void tableDoubleClicked(int, int);
    void checkStateChanged(bool);

signals:
    void contentUpdated(QString text);
    void editContent(QTableWidgetItem *item);

private:
    Site *m_site;
    QPushButton *m_deleteButton;
    QTableWidget *m_list;

    void addListItem(Menu *menu);
};

#endif // MENUELIST_H
