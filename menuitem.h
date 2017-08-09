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
    QString title() {return m_title;}
    QString url() {return m_url;}

private:
    QString m_title;
    QString m_url;
};

class MenuItemWrapper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    QString url(MenuItem *o) {return o->url();}
    QString title(MenuItem *o) {return o->title();}
};

#endif // MENUITEM_H
