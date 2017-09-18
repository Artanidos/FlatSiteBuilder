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

#ifndef IMAGESELECTOR_H
#define IMAGESELECTOR_H

#include <QWidget>
#include "widgets_global.h"

class WIDGETSSHARED_EXPORT ImageSelector : public QWidget
{
    Q_OBJECT

public:
    ImageSelector();

    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void setImage(QImage image);
    void setItem(QObject *item) {m_item = item;}
    QObject *item() {return m_item;}

signals:
    void clicked(ImageSelector *, Qt::MouseButton);
    void clicked();

private:
    QImage m_image;
    qreal m_width;
    qreal m_height;
    QObject *m_item;
};

#endif // IMAGESELECTOR_H
