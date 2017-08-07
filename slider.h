#ifndef SLIDER_H
#define SLIDER_H

#include "element.h"

class Slider : public Element
{
public:
    Slider();

    QString getHtml(QDomElement ele);
};

#endif // SLIDER_H
