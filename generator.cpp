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

#include "generator.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>

Generator::Generator()
{   
    context = PythonQt::self()->getMainModule();
    context.evalFile(":/python.py");
}

/*
 * Parses all *.html files for a gives path
 * and translates them to html into a directory named "site".
 */
void Generator::generateSite(QString path)
{
    QDir old(path + "/site");
    old.removeRecursively();

    QDir dir(path);
    dir.mkdir("site");

    QStringList filter;
    filter << "*.html";

    QFile config(path + "/config.yaml");
    if(config.open(QFile::ReadOnly))
    {
        QString data = QString::fromLatin1(config.readAll());
        sitevars = parseYaml(data);
        if(sitevars["theme"] == QVariant())
            sitevars["theme"] = "default";
        config.close();
    }
    globals.insert("site", sitevars);

    copyPath(path + "/themes/" + sitevars["theme"].toString() + "/assets", path + "/site/assets");

    foreach (QString filename, dir.entryList(filter, QDir::Files))
    {
        QFile in(path + "/" + filename);
        if(in.open(QFile::ReadOnly))
        {
            pagevars.clear();
            parseFront(QString::fromLatin1(in.readAll()));
            QString layout = pagevars["layout"].toString();
            if(layout == "")
                layout = "default";
            layout = "layouts/" + layout + ".html";

            QString name = path + "/site/" + filename.replace(".md", ".html");
            pagevars["url"] = filename.replace(".md", ".html");
            globals.insert("page", pagevars);

            QString content = translateContent(pagevars["content"].toString());
            pagevars["content"] = content;

            QFile out(name);
            if(out.open(QFile::WriteOnly))
            {
                QVariantList args;
                args << path << layout << globals << pagevars;
                QVariant rc = context.call("translateTemplate", args);
                out.write(rc.toByteArray());
                out.close();
                qInfo() << "Created file " + name;
            }
            else
                qWarning() << "Unable to create file " +  name;
        }
        else
            qWarning() << "Unable to open file " + filename;
    }
}

void Generator::parseFront(QString content)
{
    if(content.startsWith("---"))
    {
        int end = content.indexOf("---", 5);
        if(end < 0)
            return;
        QString code = content.mid(4, end - 5);
        pagevars = parseYaml(code);
        pagevars.insert("content", content.mid(end + 4));
    }
}

QVariantMap Generator::parseYaml(QString code)
{
    QVariantList args;
    args << code;
    QVariant rc = context.call("parseYaml", args);
    return rc.toMap();
}

QString Generator::translateContent(QString content)
{
    QVariantList args;
    args << content << globals << pagevars;
    QVariant rc = context.call("translateContent", args);
    return rc.toString();
}

void Generator::copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (! dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src+ QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files))
    {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}
