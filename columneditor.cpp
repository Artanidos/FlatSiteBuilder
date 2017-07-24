#include "columneditor.h"
#include "droparea.h"
#include "widgetmimedata.h"
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QTest>

ColumnEditor::ColumnEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);
    setMinimumWidth(120);
    setMinimumHeight(50);
    setMaximumHeight(50);

    QVBoxLayout *layout= new QVBoxLayout();
    layout->addWidget(new QLabel("Text"));
    setLayout(layout);
}

void ColumnEditor::mousePressEvent(QMouseEvent *event)
{
    DropArea *a = new DropArea();
    parentWidget()->layout()->replaceWidget(this, a);

    WidgetMimeData *mimeData = new WidgetMimeData();
    mimeData->setData(this);

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos());
    drag->setPixmap(pixmap);

    if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction))
    {
        delete this;
        return;
    }
}
