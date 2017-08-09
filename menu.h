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

private:
    QString m_name;
    QList<MenuItem *> m_items;
};

#endif // MENU_H
