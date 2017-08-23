/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "flatbutton.h"
#include <QMouseEvent>

FlatButton::FlatButton(QString normalIcon, QString hoverIcon, QString pressedIcon)
{
    m_normalIcon = QImage(normalIcon);
    m_hoverIcon = QImage(hoverIcon);
    if(pressedIcon.isEmpty())
        m_pressedIcon = QImage(hoverIcon);
    else
        m_pressedIcon = QImage(pressedIcon);

    setPixmap(QPixmap::fromImage(m_normalIcon));
    setCursor(Qt::PointingHandCursor);
}

void FlatButton::mousePressEvent(QMouseEvent *event)
{
    setPixmap(QPixmap::fromImage(m_pressedIcon));
    setFocus();
    event->accept();
}

void FlatButton::mouseReleaseEvent(QMouseEvent *event)
{
    setPixmap(QPixmap::fromImage(m_hoverIcon));
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
