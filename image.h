#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include "element.h"

class Image : public Element
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource)

public:
    Image();

    QString source() {return m_source;}
    void setSource(QString source) {m_source = source;}

    QString getHtml();

private:
    QString m_source;
};

#endif // IMAGE_H
