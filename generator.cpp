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
#include "sectionpropertyeditor.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QProcess>
#include <QStack>
#include <QDomDocument>
#include "mainwindow.h"

Q_DECLARE_METATYPE(QFile*)

Generator::Generator()
{
    m_themePath = QDir::homePath() + "/FlatSiteBuilder/themes";
    m_sitesPath = QDir::homePath() + "/FlatSiteBuilder/sites";
}

/*
 * Parses all *.html files for a gives path
 * and translates them to html into a directory named "site".
 */
void Generator::generateSite(Site *site, Content *contentToBuild)
{
    m_site = site;

    if(contentToBuild == 0)
    {

        QDir old;
        old.setPath(m_sitesPath + "/" + m_site->title() + "/assets");
        old.removeRecursively();
        old.cdUp();
        foreach(QString file, old.entryList(QDir::NoDotAndDotDot | QDir::Files))
        {
            old.remove(file);
        }
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
            menuitem["icon"] = item->icon();
            QVariantList subitems;
            foreach(MenuItem *subitem, item->items())
            {
                QVariantMap submenuitem;
                submenuitem["title"] = subitem->title();
                submenuitem["url"] = subitem->url();
                submenuitem["icon"] = subitem->icon();
                subitems.append(submenuitem);
            }
            menuitem["items"] = subitems;
            menuitem["hasItems"] = subitems.count() > 0 ? "true" : "false";
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
    sitevars["author"] = m_site->author();

    QDir siteDir(m_sitesPath + "/" + m_site->title());
    bool copyAssets = false;
    if(!siteDir.exists())
    {
        siteDir.setPath(m_sitesPath);
        siteDir.mkdir(m_site->title());
        copyAssets = true;
    }
    if(contentToBuild == 0 || copyAssets)
    {
        // first copy assets from site, they will not be overridden by theme assets
        copyPath(m_site->path() + "/assets", m_sitesPath + "/" + m_site->title() + "/assets");
        copyPath(m_themePath + "/" + m_site->theme() + "/assets", m_sitesPath + "/" + m_site->title() + "/assets");
    }

    int pa = 0;
    int po = 0;
    foreach (Content *content, m_site->contents())
    {
        QString subdir;
        QVariantMap cm;
        if(content->contentType() == ContentType::Page)
        {
            subdir = "pages";
            cm = sitevars["pages"].toList().at(pa++).toMap();
        }
        else
        {
            subdir = "posts";
            cm = sitevars["posts"].toList().at(po++).toMap();
        }

        if(contentToBuild != 0 && contentToBuild != content)
            continue;

        QDomDocument doc;
        QFile file(m_site->path() + "/" + subdir + "/" + content->source());
        if (file.open(QIODevice::ReadOnly))
        {
            if (doc.setContent(&file))
            {
                file.close();
                QString cnt = "";
                QDomElement root = doc.documentElement();
                QDomElement section = root.firstChildElement("Section");
                while(!section.isNull())
                {
                    cnt += SectionPropertyEditor::getHtml(section);
                    section = section.nextSiblingElement("Section");
                }

                pluginvars.clear();
                foreach(QString key, MainWindow::pluginNames())
                {
                    EditorInterface *editor = MainWindow::getPlugin(key);
                    if(root.elementsByTagName(editor->tagName()).count() > 0)
                    {
                        pluginvars["styles"] = pluginvars["styles"].toString() + editor->pluginStyles();
                        pluginvars["scripts"] = pluginvars["scripts"].toString() + editor->pluginScripts();
                    }
                }

                QString layout = content->layout();
                if(layout == "")
                    layout = "default";
                layout = layout + ".html";

                QString name = m_sitesPath + "/" + m_site->title() + "/" + content->url();

                pagevars = cm;
                QVariantMap vars;
                pagevars["content"] = translateContent(cnt, vars);

                QFile out(name);
                if(out.open(QFile::WriteOnly))
                {
                    QString rc = translateTemplate(layout, Layout);
                    out.write(translateContent(rc, vars).toUtf8());
                    out.close();
                    qInfo() << "Created file " + name;
                }
                else
                    qWarning() << "Generator::generateSite(): Unable to create file " +  name;
            }
            else
                qWarning() << "Generator::generateSite(): Unable to parse file " + m_site->path() + "/" + subdir + "/" + content->source();
        }
        else
            qWarning() << "Generator::generateSite(): Unable to open file " + m_site->path() + "/" + subdir + "/" + content->source();
    }
}

QString Generator::translateTemplate(QString layout, Mode mode)
{
    QString rc = "";
    QString path = m_site->path() + (mode == Layout ? "/layouts/" : "/includes/") + layout;

    QFile site(path);
    if(!site.exists())
        path = m_themePath + "/" + m_site->theme() + (mode == Layout ? "/layouts/" : "/includes/") + layout;

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
            if(content.at(end) == "\n")
                end++;
            rc += content.mid(end);
        }
        else
            rc = content;
    }
    else
        qWarning() << "Unable to open template " + m_themePath + "/" + layout;
    return rc;
}

class IfVars
{
public:
    bool m_isTrue;
    bool m_inIf;
    bool m_inElse;
    QString m_ifContent;
    QString m_elseContent;
};

QString Generator::translateContent(QString content, QVariantMap vars)
{
    int state = NormalState;
    int len = content.length();
    int start = 0;
    int pos = 0;
    QString rc = "";
    QString loopContent;
    QString loopVar;
    QString loopArray;
    int loopDeep = 0;
    QStack<IfVars*> stack;
    IfVars *ifvars = new IfVars;
    ifvars->m_inIf = false;
    ifvars->m_inElse = false;
    ifvars->m_isTrue = false;
    QVariantMap loopvars;
    loopvars.unite(vars);

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
                        // {% if bla == blub %} {{ if }} {% else %} {{ else }} {% endif %}
                        else if(content.mid(pos, 10) == "{% else %}")
                        {
                            if(ifvars->m_isTrue && ifvars->m_inIf)
                                rc += ifvars->m_ifContent;
                            ifvars->m_inIf = false;
                            ifvars->m_inElse = true;
                            pos += 10;
                            break;
                        }
                        else if(content.mid(pos, 11) == "{% endif %}")
                        {
                            if(ifvars->m_inIf)
                            {
                                if(ifvars->m_isTrue)
                                    rc += ifvars->m_ifContent;
                            }
                            else if(ifvars->m_inElse)
                            {
                                if(!ifvars->m_isTrue)
                                    rc += ifvars->m_elseContent;
                            }
                            ifvars->m_inIf = false;
                            ifvars->m_inElse = false;
                            ifvars->m_isTrue = false;
                            pos += 11;
                            if(!stack.isEmpty())
                                ifvars = stack.pop();
                            break;
                        }
                        else if(content.mid(pos, 2) == "{%")
                        {
                            state = InExpression;
                            break;
                        }
                        if(ifvars->m_inIf)
                            ifvars->m_ifContent += ch;
                        else if(ifvars->m_inElse)
                            ifvars->m_elseContent += ch;
                        else
                            rc += ch;
                        pos++;
                        break;
                    }
                    else
                    {
                        if(ifvars->m_inIf)
                            ifvars->m_ifContent += ch;
                        else if(ifvars->m_inElse)
                            ifvars->m_elseContent += ch;
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
                QString var = translateVar(varname, loopvars).toString();
                if(ifvars->m_inIf)
                    ifvars->m_ifContent += var;
                else if(ifvars->m_inElse)
                    ifvars->m_elseContent += var;
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
                    // found nested if statements so we push the ifvars on the stack
                    if(ifvars->m_inIf)
                    {
                        if(ifvars->m_isTrue)
                        {
                            stack.push(ifvars);
                            ifvars = new IfVars;
                        }
                        else
                        {
                            // skip to endif
                            while (pos < len)
                            {
                                if(content.mid(pos, 11) == "{% endif %}")
                                {
                                    pos += 11;
                                }
                                else
                                    pos++;
                            }
                            break;
                        }
                    }
                    else if(ifvars->m_inElse)
                    {
                        if(ifvars->m_isTrue)
                        {
                            // skip to endif
                            while (pos < len)
                            {
                                if(content.mid(pos, 11) == "{% endif %}")
                                {
                                    pos += 11;
                                }
                                else
                                    pos++;
                            }
                            break;
                        }
                        else
                        {
                            stack.push(ifvars);
                            ifvars = new IfVars;
                        }
                    }
                    int equal = exp.indexOf("==");
                    if(equal > 0)
                    {
                        QString leftVar = exp.mid(2, equal - 2).trimmed();
                        QString rightVar = exp.mid(equal + 2).trimmed();
                        QString left = translateVar(leftVar, loopvars).toString();
                        QString right = translateVar(rightVar, loopvars).toString();
                        ifvars->m_isTrue = left == right;
                    }
                    else
                    {
                        int notequal = exp.indexOf("!=");
                        if(notequal > 0)
                        {
                            QString leftVar = exp.mid(2, notequal - 2).trimmed();
                            QString rightVar = exp.mid(notequal + 2).trimmed();
                            QString left = translateVar(leftVar, loopvars).toString();
                            QString right = translateVar(rightVar, loopvars).toString();
                            ifvars->m_isTrue = left != right;
                        }
                    }
                    ifvars->m_inIf = true;
                    ifvars->m_inElse = false;
                    ifvars->m_ifContent = "";
                    ifvars->m_elseContent = "";

                    if(content.at(pos) == "\n")
                        pos++;
                }
                break;
            }
            case InLoop:
            {
                while (pos < len)
                {
                    if(content.mid(pos, 7) == "{% for ")
                    {
                        // found nested for
                        loopDeep++;
                        loopContent += content.mid(pos, 7);
                        pos += 7;
                    }
                    else if(content.mid(pos, 12) == "{% endfor %}")
                    {
                        if(loopDeep == 0)
                        {
                            QVariantList list = translateVar(loopArray, loopvars).toList();
                            for(int i = 0; i < list.count(); i++)
                            {
                                loopvars[loopVar] = list.at(i);
                                QString cnt = translateContent(loopContent, loopvars);
                                if(ifvars->m_inIf)
                                    ifvars->m_ifContent += cnt;
                                else if(ifvars->m_inElse)
                                    ifvars->m_elseContent += cnt;
                                else
                                    rc += cnt;
                            }
                            state = NormalState;
                        }
                        else
                        {
                            loopDeep--;
                            loopContent += content.mid(pos, 12);
                        }
                        pos += 12;
                        if(content.at(pos) == "\n")
                            pos++;
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

QVariant Generator::translateVar(QString exp, QVariantMap loopvars)
{
    QString indexValue = "";

    // constant string
    if(exp.startsWith("\"") && exp.endsWith("\""))
        return exp.mid(1, exp.length() - 2);

    int posOpen = exp.indexOf("[");
    if(posOpen > 0)
    {
        int posClose = exp.indexOf("]");
        QString index = exp.mid(posOpen + 1, posClose - posOpen - 1).trimmed();
        QString var = exp.mid(0, posOpen).trimmed();
        indexValue = translateVar(index, loopvars).toString();
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
    else if(exp.startsWith("plugin."))
    {
        return pluginvars[exp.mid(7)];
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
