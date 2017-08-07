#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QWidget>
#include "site.h"

class AbstractEditor : public QWidget
{
    Q_OBJECT

public:
    AbstractEditor();

    void setSite(Site *site) {m_site = site;}
    bool changed() {return m_changed;}
    virtual void setContent(QDomElement ele){}
    virtual QDomElement content() {return m_element;}

signals:
    void close(QWidget*);

public slots:
    void contentChanged() {m_changed = true;}

protected:
    QDomElement m_element;
    bool m_changed;
    Site *m_site;
};

#endif // ABSTRACTEDITOR_H
