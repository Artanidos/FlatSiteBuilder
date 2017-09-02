#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QWidget>
#include <QDomDocument>
#include <QMap>
#include "site.h"

QT_BEGIN_NAMESPACE
class QString;
class Site;
QT_END_NAMESPACE

class EditorInterface : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)

public:
    virtual ~EditorInterface() {}
    virtual QString className() = 0;
    virtual QString displayName() = 0;
    virtual QString tagName() = 0;
    virtual void setContent(QDomElement properties) = 0;
    virtual QImage icon() = 0;
    virtual QString getHtml(QDomElement properties) = 0;
    virtual QString pluginStyles() {return "";}
    virtual QString pluginScripts() {return "";}
    virtual void installAssets(QString assetsPath) {Q_UNUSED(assetsPath)}

    void setX(int x) {move(x, y());}
    void setY(int y) {move(x(), y);}
    void setWidth(int w) {resize(w, height());}
    void setHeight(int h) {resize(width(), h);}
    void setSite(Site *site) {m_site = site;}
    bool changed() {return m_changed;}
    QDomElement content() {return m_element;}

signals:
    void close();

public slots:
    void contentChanged() {m_changed = true;}

protected:
    QDomDocument m_doc;
    QDomElement m_element;
    bool m_changed;
    Site *m_site;
};

QT_BEGIN_NAMESPACE
#define EditorInterface_iid "org.crowdware.FlatSiteBuilder.EditorInterface"

Q_DECLARE_INTERFACE(EditorInterface, EditorInterface_iid)
QT_END_NAMESPACE

#endif // INTERFACES_H
