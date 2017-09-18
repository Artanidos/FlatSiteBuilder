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

ImageSelector::ImageSelector()
{
    setCursor(Qt::PointingHandCursor);
}

void ImageSelector::setImage(QImage image)
{
    m_image = image;
    update();
}

void ImageSelector::mousePressEvent(QMouseEvent *event)
{
    event->accept();
}

void ImageSelector::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
    if(m_item)
        emit clicked(this, event->button());
    else
        emit clicked();
}

void ImageSelector::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QSize pixSize = m_image.size();
    pixSize.scale(event->rect().size(), Qt::KeepAspectRatio);
    QImage scaledImage = m_image.scaled(pixSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qreal x = (event->rect().size().width() - scaledImage.size().width()) / 2.0;
    qreal y = (event->rect().size().height() - scaledImage.size().height()) / 2.0;
    painter.drawImage(x, y, scaledImage);
}
