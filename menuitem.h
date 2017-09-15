#ifndef MENUITEM_H
#define MENUITEM_H

#include <QObject>

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

private:
    QString m_title;
    QString m_url;
    QString m_icon;
    QList<MenuItem *> m_items;
    bool m_isSubitem;
    MenuItem *m_parentItem;
};

#endif // MENUITEM_H
