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

#include "column.h"
#include "plugins.h"
#include "interfaces.h"
#include "mainwindow.h"
#include <QXmlStreamReader>
#include <QTest>

QString Column::getHtml(QXmlStreamReader *xml)
{
    QString span = xml->attributes().value("span").toString();
    QString html = "<div class=\"col-md-" + span + "\">\n";
    while(xml->readNextStartElement())
    {
        QString pluginName = xml->name() + "Editor";
        if(Plugins::hasPlugin(pluginName))
        {
            html += Plugins::getPlugin(pluginName)->getHtml(xml);
            Plugins::addUsedPlugin(pluginName);
        }
        else
            qDebug() << "Undefined element " + pluginName;
        xml->readNext();
    }
    return html + "\n</div>\n";
}
