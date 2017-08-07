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

#ifndef SLIDEREDITOR_H
#define SLIDEREDITOR_H

#include "abstracteditor.h"

class SliderEditor : public AbstractEditor
{
    Q_OBJECT

public:
    SliderEditor();

    void setContent(QDomElement ele);
    QDomElement content() {return m_element;}

private slots:
    void save();
    void cancel();
};

#endif // SLIDEREDITOR_H
