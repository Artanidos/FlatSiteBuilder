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

#ifndef ANIMATIONLABEL_H
#define ANIMATIONLABEL_H

#include <QLabel>

class AnimationLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)

public:
    AnimationLabel();

    void setX(int x) {move(x, y());}
    void setY(int y) {move(x(), y);}
    void setWidth(int w) {resize(w, height());}
    void setHeight(int h) {resize(width(), h);}
};

#endif // ANIMATIONLABEL_H
