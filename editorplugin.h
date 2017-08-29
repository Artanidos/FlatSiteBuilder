#ifndef EDITORPLUGIN_H
#define EDITORPLUGIN_H

#include <QQuickWidget>
#include "abstracteditor.h"

class EditorPlugin : public QQuickWidget
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)

public:
    EditorPlugin();

    void setX(int x) {move(x, y());}
    void setY(int y) {move(x(), y);}
    void setWidth(int w) {resize(w, height());}
    void setHeight(int h) {resize(width(), h);}

    void setSite(Site *site) {m_site = site;}
    bool changed() {return m_changed;}
    void setContent(QDomElement ele){m_element = ele;}
    QDomElement content() {return m_element;}

signals:
    void close();

private:
    Site *m_site;
    bool m_changed;
    QDomElement m_element;
};

#endif // EDITORPLUGIN_H
