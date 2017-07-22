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
#include "content.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QProcess>
#include "text.h"

Q_DECLARE_METATYPE(QFile*)

Generator::Generator(PythonQtObjectPtr ctx)
{
    context = ctx;
}

/*
 * Parses all *.html files for a gives path
 * and translates them to html into a directory named "site".
 */
void Generator::generateSite(Site *site)
{
    // TODO: themes should be deployed and we should use the app path
    QString theme_path = "/home/olaf/SourceCode/FlatSiteBuilder/themes/";
    m_site = site;

    QString dir = QDir::homePath() + "/FlatSiteBuilder";
    QDir old;
    old.setPath(dir + "/" + m_site->title() + "/assets");
    old.removeRecursively();

    foreach(QString file, old.entryList())
    {
        old.remove(file);
    }

    QVariantList pages;
    QVariantList posts;
    foreach (Content *content, m_site->contents())
    {
        if(content->contentType() == ContentType::Page)
            pages.append(QVariant::fromValue(content));
        else
            posts.append(QVariant::fromValue(content));
    }
    sitevars["pages"] = pages;
    sitevars["posts"] = posts;
    sitevars["theme"] = m_site->theme();
    sitevars["copyright"] = m_site->copyright();
    sitevars["source"] = m_site->path();
    globals.insert("site", sitevars);

    // first copy assets from site, they will not be overridden by theme assets
    copyPath(m_site->path() + "/assets", dir + "/" + m_site->title() + "/assets");
    copyPath(theme_path + sitevars["theme"].toString() + "/assets", dir + "/" + m_site->title() + "/assets");

    foreach (Content *content, m_site->contents())
    {
        QString subdir;
        if(content->contentType() == ContentType::Page)
            subdir = "pages";
        else
            subdir = "posts";
        QFile in(m_site->path() + "/" + subdir + "/" + content->source());
        if(in.open(QFile::ReadOnly))
        {
            pagevars.clear();
            QString cnt = QString::fromLatin1(in.readAll());
            QString layout = content->layout();
            if(layout == "")
                layout = "default";
            layout = layout + ".html";

            QString name = dir + "/" + m_site->title() + "/" + content->source().replace(".md", ".html");
            pagevars["url"] = content->source().replace(".md", ".html");
            pagevars["title"] = content->title();
            pagevars["date"] = content->date();
            pagevars["author"] = content->author();
            pagevars["excerpt"] = content->excerpt();

            globals.insert("page", pagevars);

            pagevars["content"] = translateContent(cnt);

            QFile out(name);
            if(out.open(QFile::WriteOnly))
            {
                QVariantList args;
                args << theme_path << layout << globals << pagevars;
                QVariant rc = context.call("translateTemplate", args);
                out.write(rc.toByteArray());
                out.close();
                qInfo() << "Created file " + name;
            }
            else
                qWarning() << "Unable to create file " +  name;
        }
        else
            qWarning() << "Unable to open file " + content->source();
    }
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
