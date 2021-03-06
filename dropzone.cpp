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

#include "dropzone.h"
#include <QHBoxLayout>
#include <QLabel>

DropZone::DropZone(int w, int h)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()).lighter().lighter());
    setPalette(pal);
    setAutoFillBackground(true);
    setMinimumWidth(w);
    setMaximumWidth(w);
    setMinimumHeight(h);
    setMaximumHeight(h);
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addStretch(1);
    hbox->addWidget(new QLabel("Drop Here"));
    hbox->addStretch(1);
    setLayout(hbox);
}
