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
