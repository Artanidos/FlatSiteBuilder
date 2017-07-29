#ifndef WIDGETMIMEDATA_H
#define WIDGETMIMEDATA_H

#include <QMimeData>
#include "elementeditor.h"

class WidgetMimeData : public QMimeData
{
    Q_OBJECT
public:
    WidgetMimeData();

    void setData(QWidget *);
    QWidget *getData() const;

    void setSize(int w, int h) {m_width =  w; m_height = h;}
    int width() const {return m_width;}
    int height() const {return m_height;}

private:
    QWidget *m_data;
    int m_width;
    int m_height;
};

#endif // WIDGETMIMEDATA_H
