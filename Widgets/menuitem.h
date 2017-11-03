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

#ifndef MENUITEM_H
#define MENUITEM_H

#include <QObject>
#include <QHash>

class MenuItem : public QObject
{
    Q_OBJECT

public:
    MenuItem();

    void setTitle(QString title) {m_title = title;}
    void setUrl(QString url) {m_url = url;}
    void setIcon(QString icon) {m_icon = icon;}
    QString title() {return m_title;}
    QString url() {return m_url;}
    QString icon() {return m_icon;}
    MenuItem *parentItem() {return m_parentItem;}
    void setParentItem(MenuItem *parent) {m_parentItem = parent;}
    bool isSubitem() {return m_isSubitem;}
    void setSubitem(bool sub) {m_isSubitem = sub;}
    void addMenuitem(MenuItem *item) {m_items.append(item); item->setParentItem(this);}
    void removeMenuitem(MenuItem *item) {m_items.removeOne(item); item->setParentItem(NULL);}
    QList<MenuItem *> items() {return m_items;}
    void addAttribute(QString attName, QString value) {m_attributes.insert(attName, value);}
    QHash<QString,QString> attributes() {return m_attributes;}

private:
    QString m_title;
    QString m_url;
    QString m_icon;
    QList<MenuItem *> m_items;
    bool m_isSubitem;
    MenuItem *m_parentItem;
    QHash<QString,QString> m_attributes;
};

#endif // MENUITEM_H
