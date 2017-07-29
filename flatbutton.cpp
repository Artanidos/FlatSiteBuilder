#include "flatbutton.h"
#include <QMouseEvent>

FlatButton::FlatButton(QString normalIcon, QString hoverIcon)
{
    m_normalIcon = QImage(normalIcon);
    m_hoverIcon = QImage(hoverIcon);

    setPixmap(QPixmap::fromImage(m_normalIcon));
    setCursor(Qt::PointingHandCursor);
}

void FlatButton::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void FlatButton::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    emit clicked();
}

void FlatButton::enterEvent(QEvent * event)
{
    setPixmap(QPixmap::fromImage(m_hoverIcon));
    QWidget::enterEvent(event);
}

void FlatButton::leaveEvent(QEvent * event)
{
    setPixmap(QPixmap::fromImage(m_normalIcon));
    QWidget::leaveEvent(event);
}
