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
#include "text.h"
#include "image.h"
#include "slider.h"
#include <QTest>

Column::Column()
{

}

QString Column::getHtml(QDomElement col)
{
    QString span = col.attribute("span", "1");
    QString html = "<div class=\"col-md-" + span + "\">\n";
    QDomElement ele = col.firstChildElement();
    while(!ele.isNull())
    {
        if(ele.nodeName() == "Text")
        {
            Text *t = new Text();
            html += t->getHtml(ele);
        }
        else if(ele.nodeName() == "Image")
        {
            Image *i = new Image();
            html += i->getHtml(ele);
        }
        else if(ele.nodeName() == "Slider")
        {
            Slider *s = new Slider();
            html += s->getHtml(ele);
        }
        else
            qDebug() << "Undefined element " + ele.nodeName();
        ele = ele.nextSiblingElement();
    }
    return html + "\n</div>\n";
}
