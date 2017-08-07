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

#include "image.h"
#include <QTest>

Image::Image()
{

}

QString Image::getHtml(QDomElement ele)
{
    QString source = ele.attribute("src");
    QString animation = ele.attribute("animation");
    QString url = source.mid(source.indexOf("assets/images/"));
    if(animation == "none")
        return "<img class=\"img-responsive pull-left inner\" src=\"" + url + "\">";
    else
        return "<img class=\"img-responsive appear-animation pull-left inner\" src=\"" + url + "\" data-animation=\"" + animation + "\">";
}
