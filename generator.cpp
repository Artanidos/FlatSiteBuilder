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
#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>

Generator::Generator()
{   
    vars.insert("name", "Hans");
    vars.insert("nachname", "Meiser");
}

void Generator::generateSite(QString path)
{
    PythonQtObjectPtr context = PythonQt::self()->getMainModule();
    context.evalFile(":/python.py");

    QDir old(path + "/site");
    old.removeRecursively();

    QDir dir(path);
    dir.mkdir("site");

    QStringList filter;
    filter << "*.md";
    filter << "*.html";
    foreach (QString filename, dir.entryList(filter, QDir::Files))
    {
        QString name = path + "/site/" + filename.replace(".md", ".html");
        QFile out(name);
        if(out.open(QFile::WriteOnly))
        {
            QVariantList args;
            args << path << filename << vars;
            QVariant rc = context.call("translate", args);
            out.write(rc.toByteArray());
            out.close();
        }
        else
            qDebug() << "Unable to create file " +  name;
    }
}
