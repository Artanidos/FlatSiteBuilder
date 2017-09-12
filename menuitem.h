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
    void addMenuitem(MenuItem *item) {m_items.append(item);}
    QList<MenuItem *> items() {return m_items;}

private:
    QString m_title;
    QString m_url;
    QString m_icon;
    QList<MenuItem *> m_items;
};

#endif // MENUITEM_H
