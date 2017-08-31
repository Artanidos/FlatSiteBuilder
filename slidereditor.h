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

#include "interfaces.h"
#include <QPushButton>
#include <QTableWidget>

class SliderEditor : public EditorInterface
{
    Q_OBJECT

public:
    SliderEditor();

    void setContent(QDomElement ele);
    QString className() {return "SliderEditor";}
    QString displayName() {return "Slider";}
    QString tagName() {return "Slider";}
    QImage icon() {return QImage(":/images/slider.png");}
    QString getHtml(QDomElement ele, QMap<QString, QObject*> plugins);

signals:
    void close();

private:
    QPushButton *m_deleteButton;
    QTableWidget *m_list;
};

#endif // SLIDEREDITOR_H
