#include "widgetmimedata.h"

WidgetMimeData::WidgetMimeData()
{

}

void WidgetMimeData::setData(QWidget *widget)
{
    m_data = widget;
}

QWidget *WidgetMimeData::getData() const
{
    return m_data;
}
