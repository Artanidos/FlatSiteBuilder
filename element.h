#ifndef ELEMENT_H
#define ELEMENT_H

#include <QObject>

class Element : public QObject
{
    Q_OBJECT

public:
    Element();

    virtual QString getHtml();
};

#endif // ELEMENT_H
