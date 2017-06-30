#ifndef TEXT_H
#define TEXT_H

#include <QObject>
#include "element.h"

class Text : public Element
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    Text();

    QString text() {return m_text;}
    void setText(QString text) {m_text = text;}

private:
    QString m_text;
};

#endif // TEXT_H
