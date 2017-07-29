#include "widgetmimedata.h"

WidgetMimeData::WidgetMimeData()
{

}

void WidgetMimeData::setData(QWidget *ce)
{
    m_data = ce;
}

QWidget *WidgetMimeData::getData() const
{
    return m_data;
}
