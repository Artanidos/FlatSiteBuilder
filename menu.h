#ifndef MENU_H
#define MENU_H

#include <QObject>
#include "menuitem.h"

class Menu : public QObject
{
    Q_OBJECT

public:
    Menu();

    void setName(QString name) {m_name = name;}
    QString name() {return m_name;}
    void addMenuitem(MenuItem *item) {m_items.append(item);}
    QList<MenuItem *> items() {return m_items;}
    void removeItem(MenuItem *item) {m_items.removeOne(item);}
    int id() {return m_id;}
    void setId(int id) {m_id = id;}

private:
    QString m_name;
    QList<MenuItem *> m_items;
    int m_id;
};

#endif // MENU_H
