#include "columneditor.h"
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>

ColumnEditor::ColumnEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);
    setMinimumWidth(60);
    setMinimumHeight(160);
    setMaximumHeight(160);

    QVBoxLayout *layout= new QVBoxLayout();
    layout->addWidget(new QLabel("Text"));
    setLayout(layout);
}

void ColumnEditor::mousePressEvent(QMouseEvent *event)
{
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << 13;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(QStringLiteral("FlatSiteBuilder/ColumnEditor"), itemData);

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos());
    drag->setPixmap(pixmap);

    if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction))
    {

    }
}
