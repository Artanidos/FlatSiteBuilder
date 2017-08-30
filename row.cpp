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

#include "row.h"
#include "column.h"

Row::Row()
{

}

QString Row::getHtml(QDomElement row)
{
    QString cls = row.attribute("cssclass");
    QString html = "<div class=\"row" + (cls.isEmpty() ? "" : " " + cls) + "\">\n";
    QDomElement col = row.firstChildElement("Column");
    while(!col.isNull())
    {
        Column *c = new Column();
        html += c->getHtml(col);
        col = col.nextSiblingElement("Column");
    }
    return html + "</div>\n";
}
