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
    void tableDoubleClicked(int, int);
    void deleteMenu(QObject *menu);
    void editMenu(QObject *menu);

signals:
    void contentUpdated(QString text);
    void editContent(QTableWidgetItem *item);

private:
    Site *m_site;
    QTableWidget *m_list;

    void addListItem(Menu *menu);
};

#endif // MENUELIST_H
