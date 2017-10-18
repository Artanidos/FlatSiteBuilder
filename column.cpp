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

QString Column::getHtml(QXmlStreamReader *xml, QString filename)
{
    QString span = xml->attributes().value("span").toString();
    QString html = "<div class=\"col-md-" + span + "\">\n";
    while(xml->readNext())
    {
        if(xml->isStartElement())
        {
            QString pluginName = xml->name() + "Editor";
            if(Plugins::hasElementPlugin(pluginName))
            {
                html += Plugins::getElementPlugin(pluginName)->getHtml(xml);
                Plugins::addUsedPlugin(pluginName);
            }
            else
                qWarning() << "Undefined tag " + xml->name() + " in line " + QString::number(xml->lineNumber()) + " in file " + filename;
        }
        else if(xml->isEndElement() || xml->atEnd())
            break;
    }
    return html + "\n</div>\n";
}
