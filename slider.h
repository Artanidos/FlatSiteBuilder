#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>

class QDomElement;
class Slider : public QObject
{
public:
    Slider() {}

    QString getHtml(QDomElement ele);
};

#endif // SLIDER_H
