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

#include <QtPlugin>
#include <QObject>
#include <QTableWidget>
#include <QPushButton>
#include "interfaces.h"

class SliderEditor : public EditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.EditorInterface" FILE "slidereditor.json")
    Q_INTERFACES(EditorInterface)

public:
    SliderEditor();
    QString className() {return "SliderEditor";}
    QString displayName() {return "Slider";}
    QString tagName() {return "Slider";}
    QImage icon() {return QImage(":/slider.png");}
    QString getHtml(QDomElement ele, QMap<QString, EditorInterface*> plugins);
    void setContent(QDomElement ele);

private slots:
    void closeEditor();

private:
    QPushButton *m_deleteButton;
    QLineEdit *m_adminlabel;
    QTableWidget *m_list;
};

#endif // SLIDEREDITOR_H
