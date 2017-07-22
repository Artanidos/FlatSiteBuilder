#include "roweditor.h"
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QMimeData>

RowEditor::RowEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().alternateBase().color()));
    setPalette(pal);
    setAutoFillBackground(true);
    setMaximumHeight(180);

    m_layout = new QGridLayout();
    setLayout(m_layout);
}

void RowEditor::addColumn(ColumnEditor *ce, int column)
{
    m_layout->addWidget(ce, 0, column);
}

void RowEditor::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat(QStringLiteral("FlatSiteBuilder/ColumnEditor")))
        event->accept();
    else
        event->ignore();
}

void RowEditor::dragLeaveEvent(QDragLeaveEvent *event)
{
    //QRect updateRect = highlightedRect;
    //highlightedRect = QRect();
    //update(updateRect);
    event->accept();
}

void RowEditor::dragMoveEvent(QDragMoveEvent *event)
{
    //QRect updateRect = highlightedRect.united(targetSquare(event->pos()));

    if (event->mimeData()->hasFormat(QStringLiteral("FlatSiteBuilder/ColumnEditor")))
    {
        //&& pieceRects.indexOf(targetSquare(event->pos())) == -1) {

        //highlightedRect = targetSquare(event->pos());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        //highlightedRect = QRect();
        event->ignore();
    }

    //update(updateRect);
}

void RowEditor::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(QStringLiteral("FlatSiteBuilder/ColumnEditor")))
    {
        //&& pieceRects.indexOf(targetSquare(event->pos())) == -1) {

        //QByteArray pieceData = event->mimeData()->data(PiecesList::puzzleMimeType());
        //QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        //QRect square = targetSquare(event->pos());
        //QPixmap pixmap;
        //QPoint location;
        //dataStream >> pixmap >> location;

        //pieceLocations.append(location);
        //piecePixmaps.append(pixmap);
        //pieceRects.append(square);

        //highlightedRect = QRect();
        //update(square);

        event->setDropAction(Qt::MoveAction);
        event->accept();

        //if (location == QPoint(square.x()/pieceSize(), square.y()/pieceSize())) {
        //    inPlace++;
        //    if (inPlace == 25)
        //        emit puzzleCompleted();
        //}
    }
    else
    {
        //highlightedRect = QRect();
        event->ignore();
    }
}
