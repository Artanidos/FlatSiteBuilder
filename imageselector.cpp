#include "imageselector.h"
#include <QMouseEvent>

ImageSelector::ImageSelector()
{
    setCursor(Qt::PointingHandCursor);
}

void ImageSelector::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void ImageSelector::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    emit clicked();
}
