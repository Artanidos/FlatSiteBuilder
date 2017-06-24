/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "generator.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>

Generator::Generator()
{
}

void Generator::generateSite(QString path)
{
    QDir dir(path);
    dir.mkdir("_site");
    QStringList filter;
    filter << "*.md";
    filter << "*.html";
    foreach (QString filename, dir.entryList(filter, QDir::Files))
    {
        QFile in(path + "/" + filename);
        if(in.open(QFile::ReadOnly))
        {
            QString name = path + "/_site/" + filename.replace(".md", ".html");
            QFile out(name);
            if(out.open(QFile::WriteOnly))
            {
                //out.write(translate(QString::fromLatin1(in.readAll())).toLatin1());
                out.close();
                in.close();
            }
            else
                qDebug() << "Unable to create file " +  name;
        }
        else
            qDebug() << "Unable to open file " + path + "/" + filename;
    }
}
