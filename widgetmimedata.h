#ifndef WIDGETMIMEDATA_H
#define WIDGETMIMEDATA_H

#include <QMimeData>

class WidgetMimeData : public QMimeData
{
    Q_OBJECT
public:
    WidgetMimeData();

    void setData(QWidget *);
    QWidget *getData() const;

private:
    QWidget *m_data;
};

#endif // WIDGETMIMEDATA_H
