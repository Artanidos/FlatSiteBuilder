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

#include "imageselector.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTest>

ImageSelector::ImageSelector()
{
    setCursor(Qt::PointingHandCursor);
    m_width = 300;
    m_height = 300;
}

QSize ImageSelector::sizeHint() const
{
    return QSize(m_width, m_height);
}

void ImageSelector::setImage(QImage image)
{
    m_image = image;
    qreal w = image.width();
    qreal h = image.height();

    if(w > 300 || h > 300)
    {
        if(w > h)
        {
            m_width = 300;
            m_height = 300.0 / w * h;
        }
        else if (h > w)
        {
            m_height = 300;
            m_width = 300.0 / h * w;
        }
        else
        {
            m_width = 300;
            m_height = 300;
        }
    }
    else
    {
        m_width = w;
        m_height = h;
    }
    updateGeometry();
    update();
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

void ImageSelector::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawImage(QRectF(0, 0, m_width, m_height), m_image);
}
