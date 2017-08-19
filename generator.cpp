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
#include <QDomDocument>
#include "text.h"

Q_DECLARE_METATYPE(QFile*)

Generator::Generator()
{
    m_themePath = QDir::homePath() + "/FlatSiteBuilder/themes/";
}

/*
 * Parses all *.html files for a gives path
 * and translates them to html into a directory named "site".
 */
void Generator::generateSite(Site *site)
{
    m_site = site;

    QString dir = QDir::homePath() + "/FlatSiteBuilder/sites";
    QDir old;
    old.setPath(dir + "/" + m_site->title() + "/assets");
    old.removeRecursively();
    old.cdUp();
    foreach(QString file, old.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        old.remove(file);
    }

    QVariantList pages;
    QVariantList posts;
    foreach (Content *content, m_site->contents())
    {
        QVariantMap cm;
        cm["author"] = content->author();
        cm["date"] = content->date();
        cm["excerpt"] = content->excerpt();
        cm["layout"] = content->layout();
        cm["logo"] = content->logo();
        cm["menu"] = content->menu();
        cm["source"] = content->source();
        cm["title"] = content->title();
        cm["url"] = content->url();
        cm["keywords"] = content->keywords();
        if(content->contentType() == ContentType::Page)
            pages.append(cm);
        else
            posts.append(cm);
    }
    QVariantMap menus;
    foreach(Menu *menu, m_site->menus())
    {   
        QVariantList items;
        foreach(MenuItem *item, menu->items())
        {
            QVariantMap menuitem;
            menuitem["title"] = item->title();
            menuitem["url"] = item->url();
            items.append(menuitem);
        }
        menus[menu->name()] = items;
    }
    sitevars["menus"] = menus;
    sitevars["pages"] = pages;
    sitevars["posts"] = posts;
    sitevars["title"] = m_site->title();
    sitevars["description"] = m_site->description();
    sitevars["theme"] = m_site->theme();
    sitevars["theme_accent"] = m_site->themeAccent();
    sitevars["copyright"] = m_site->copyright();
    sitevars["source"] = m_site->path();
    sitevars["github"] = m_site->github();
    sitevars["keywords"] = m_site->keywords();

    // first copy assets from site, they will not be overridden by theme assets
    copyPath(m_site->path() + "/assets", dir + "/" + m_site->title() + "/assets");
    copyPath(m_themePath + m_site->theme() + "/assets", dir + "/" + m_site->title() + "/assets");

    int index = 0;
    foreach (Content *content, m_site->contents())
    {
        QString subdir;
        QVariantMap cm;
        if(content->contentType() == ContentType::Page)
        {
            subdir = "pages";
            cm = sitevars["pages"].toList().at(index).toMap();
        }
        else
        {
            subdir = "posts";
            cm = sitevars["posts"].toList().at(index).toMap();
        }
        index++;

        QDomDocument doc;
        QFile file(m_site->path() + "/" + subdir + "/" + content->source());
        if (file.open(QIODevice::ReadOnly))
        {
            if (doc.setContent(&file))
            {
                file.close();
                //pagevars.clear();
                QString cnt = "";
                QDomElement c = doc.documentElement();
                QDomElement section = c.firstChildElement("Section");
                while(!section.isNull())
                {
                    Section *sec = new Section();
                    QString fw = section.attribute("fullwidth", "false");
                    if(fw == "true")
                        sec->setFullWidth(true);
                    cnt += sec->getHtml(section);
                    section = section.nextSiblingElement("Section");
                }

                QString layout = content->layout();
                if(layout == "")
                    layout = "default";
                layout = layout + ".html";

                QString name = dir + "/" + m_site->title() + "/" + content->url();

                pagevars = cm;
                pagevars["content"] = translateContent(cnt);

                QFile out(name);
                if(out.open(QFile::WriteOnly))
                {
                    QString rc = translateTemplate(layout, Layout);
                    out.write(translateContent(rc).toUtf8());
                    out.close();
                    qInfo() << "Created file " + name;
                }
                else
                    qWarning() << "Generator::generateSite(): Unable to create file " +  name;
            }
            else
                qWarning() << "Generator::generateSite(): Unable to parse file " + content->source();
        }
        else
            qWarning() << "Generator::generateSite(): Unable to open file " + content->source();
    }
}

QString Generator::translateTemplate(QString layout, Mode mode)
{
    QString rc = "";
    QString path = m_site->path() + (mode == Layout ? "/layouts/" : "/includes/") + layout;

    QFile site(path);
    if(!site.exists())
        path = m_themePath + m_site->theme() + (mode == Layout ? "/layouts/" : "/includes/") + layout;

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QString content = QString::fromLatin1(file.readAll());
        file.close();
        int pos = content.indexOf("{% include ");
        int end = 0;
        if(pos >= 0)
        {
            while(pos >= 0)
            {
                rc += content.mid(end, pos - end);
                end = content.indexOf("%}", pos + 11) + 2;
                QString include = content.mid(pos + 11, end - pos - 13).trimmed();
                rc += translateTemplate(include, Include);
                pos = content.indexOf("{% include ", end + 1);
            }
            rc += content.mid(end);
        }
        else
            rc = content;
    }
    else
            qWarning() << "Unable to open template " + m_themePath + layout;
    return rc;
}

QString Generator::translateContent(QString content)
{
    int state = NormalState;
    int len = content.length();
    int start = 0;
    int pos = 0;
    bool inIf = false;
    bool inElse = false;
    bool isTrue = false;
    QString rc = "";
    QString ifContent;
    QString elseContent;
    QString loopContent;
    QString loopVar;
    QString loopArray;

    while (pos < len)
    {
        switch (state)
        {
            case NormalState:
            default:
                while (pos < len)
                {
                    QChar ch = content.at(pos);
                    if (ch == '{')
                    {
                        if(content.mid(pos, 2) == "{{")
                        {
                            state = InVar;
                            break;
                        }
                        // {½ if bla == blub %} {{ if }} {% else %} {{ else }} {% endif %}
                        else if(content.mid(pos, 10) == "{% else %}")
                        {
                            if(isTrue && inIf)
                                rc += ifContent;
                            inIf = false;
                            inElse = true;
                            pos += 10;
                            break;
                        }
                        else if(content.mid(pos, 11) == "{% endif %}")
                        {
                            if(inIf)
                            {
                                if(isTrue)
                                    rc += ifContent;
                            }
                            else if(inElse)
                            {
                                if(!isTrue)
                                    rc += elseContent;
                            }
                            inIf = false;
                            inElse = false;
                            isTrue = false;
                            pos += 11;
                            break;
                        }
                        else if(content.mid(pos, 2) == "{%")
                        {
                            state = InExpression;
                            break;
                        }
                        if(inIf)
                            ifContent += ch;
                        else if(inElse)
                            elseContent += ch;
                        else
                            rc += ch;
                        pos++;
                        break;
                    }
                    else
                    {
                        if(inIf)
                            ifContent += ch;
                        else if(inElse)
                            elseContent += ch;
                        else
                            rc += ch;
                        pos++;
                    }
                }
                break;

            case InVar:
            {
                start = pos;
                while (pos < len)
                {
                    if(content.mid(pos, 2) == "}}")
                    {
                        pos += 2;
                        state = NormalState;
                        break;
                    }
                    else
                        pos++;
                }
                QString varname = content.mid(start + 2, pos - start - 4).trimmed();
                QString var = translateVar(varname).toString();
                if(inIf)
                    ifContent += var;
                else if(inElse)
                    elseContent += var;
                else
                    rc += var;
                break;
            }
            case InExpression:
            {
                start = pos;
                while (pos < len)
                {
                    if(content.mid(pos, 2) == "%}")
                    {
                        pos += 2;
                        state = NormalState;
                        break;
                    }
                    else
                        pos++;
                }
                QString exp = content.mid(start + 2, pos - start - 4);
                exp = exp.trimmed();
                if(exp.startsWith("for "))
                {
                    int blank2 = exp.indexOf(" ", 5);
                    int blank3 = exp.indexOf(" in ", blank2) + 4;
                    loopVar = exp.mid(4, blank2 - 4).trimmed();
                    loopArray = exp.mid(blank3).trimmed();
                    state = InLoop;
                    loopContent = "";
                }
                else if(exp.startsWith("if "))
                {
                    int equal = exp.indexOf("==");
                    QString leftVar = exp.mid(2, equal - 2).trimmed();
                    QString rightVar = exp.mid(equal + 2).trimmed();
                    QString left = translateVar(leftVar).toString();
                    QString right = translateVar(rightVar).toString();
                    isTrue = left == right;
                    inIf = true;
                    inElse = false;
                    ifContent = "";
                    elseContent = "";
                }
                break;
            }
            case InLoop:
            {
                while (pos < len)
                {
                    if(content.mid(pos, 12) == "{% endfor %}")
                    {
                        loopvars.clear();
                        QVariantList list = translateVar(loopArray).toList();

                        for(int i = 0; i < list.count(); i++)
                        {
                            loopvars[loopVar] = list.at(i);
                            rc += translateContent(loopContent);
                        }
                        pos += 12;
                        state = NormalState;
                        break;
                    }
                    else
                    {
                        loopContent += content.at(pos);
                        pos++;
                    }
                }
                break;
            }
        }
    }
    return rc;
}

QVariant Generator::translateVar(QString exp)
{
    QString indexValue = "";

    if(exp == "\"\"") // blank constant: {% if page.keywords == "" %}
        return "";

    int posOpen = exp.indexOf("[");
    if(posOpen > 0)
    {
        int posClose = exp.indexOf("]");
        QString index = exp.mid(posOpen + 1, posClose - posOpen - 1).trimmed();
        QString var = exp.mid(0, posOpen).trimmed();
        indexValue = translateVar(index).toString();
        exp = var;
    }

    if(exp.startsWith("page."))
        return pagevars[exp.mid(5)];
    else if(exp.startsWith("site."))
    {
        if(indexValue.isEmpty())
            return sitevars[exp.mid(5)];
        else
            return sitevars[exp.mid(5)].toMap()[indexValue];
    }
    else
    {
        int dot = exp.indexOf(".");
        if(dot <= 0)
            return pagevars[exp];
        QString var = exp.mid(0, dot);
        QString value = exp.mid(dot + 1);
        QVariant obj = loopvars[var];
        QVariantMap map = obj.toMap();
        return map[value];
    }
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
