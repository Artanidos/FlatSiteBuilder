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
