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
#include "plugins.h"
#include "sectionpropertyeditor.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QProcess>
#include <QStack>
#include <QXmlStreamReader>

Q_DECLARE_METATYPE(QFile*)

Generator::Generator()
{
    m_themePath = Generator::themesPath();
    m_sitesPath = Generator::sitesPath();
}

/*
 * Parses all *.xml files for a gives path
 * and translates them to html into a directory named "site".
 */
void Generator::generateSite(MainWindow *win, Site *site, Content *contentToBuild)
{
    m_win = win;
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
    foreach (Content *content, m_site->pages())
    {
        QVariantMap cm;
        cm["author"] = content->author();
        cm["date"] = content->date();
        cm["layout"] = content->layout();
        cm["menu"] = content->menu();
        cm["source"] = content->source();
        cm["title"] = content->title();
        cm["url"] = content->url();
        cm["keywords"] = content->keywords();
        foreach(QString attName, content->additionalAttributes().keys())
        {
            cm.insert(attName, content->additionalAttributes().value(attName));
        }
        pages.append(cm);
    }
    foreach (Content *content, m_site->posts())
    {
        QVariantMap cm;
        cm["author"] = content->author();
        cm["date"] = content->date();
        cm["excerpt"] = content->excerpt();
        cm["layout"] = content->layout();
        cm["menu"] = content->menu();
        cm["source"] = content->source();
        cm["title"] = content->title();
        cm["url"] = content->url();
        cm["keywords"] = content->keywords();
        foreach(QString attName, content->additionalAttributes().keys())
        {
            cm.insert(attName, content->additionalAttributes().value(attName));
        }
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
    sitevars["copyright"] = m_site->copyright();
    sitevars["source"] = m_site->sourcePath();
    sitevars["keywords"] = m_site->keywords();
    sitevars["author"] = m_site->author();

    ThemeEditorInterface *tei = Plugins::getThemePlugin(Plugins::actualThemeEditorPlugin());
    if(tei)
    {
        tei->setWindow(m_win);
        tei->setSourcePath(m_site->sourcePath());
        themevars = tei->themeVars();
    }

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
        copyPath(m_site->sourcePath() + "/assets", m_sitesPath + "/" + m_site->title() + "/assets");
        copyPath(m_themePath + "/" + m_site->theme() + "/assets", m_sitesPath + "/" + m_site->title() + "/assets");

        foreach (Content *page, m_site->pages())
        {
            generateContent(page);
        }

        foreach (Content *post, m_site->posts())
        {
            generateContent(post);
        }
    }
    else
    {
        generateContent(contentToBuild);
    }
}

void Generator::generateContent(Content *content)
{
    QString subdir;
    QVariantMap cm;
    if(content->contentType() == ContentType::Page)
    {
        subdir = "pages";
        cm["author"] = content->author();
        cm["date"] = content->date();
        cm["layout"] = content->layout();
        cm["menu"] = content->menu();
        cm["source"] = content->source();
        cm["title"] = content->title();
        cm["url"] = content->url();
        cm["keywords"] = content->keywords();
    }
    else
    {
        subdir = "posts";
        cm["author"] = content->author();
        cm["date"] = content->date();
        cm["excerpt"] = content->excerpt();
        cm["layout"] = content->layout();
        cm["menu"] = content->menu();
        cm["source"] = content->source();
        cm["title"] = content->title();
        cm["url"] = content->url();
        cm["keywords"] = content->keywords();
    }
    foreach(QString attName, content->additionalAttributes().keys())
    {
        cm.insert(attName, content->additionalAttributes().value(attName));
    }
    QFile file(m_site->sourcePath() + "/" + subdir + "/" + content->source());
    if (file.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader xml(&file);
        QString cnt = "";
        Plugins::clearUsedPlugins();

        if(xml.readNextStartElement())
        {
            if(xml.name() == "Content")
            {
                while(xml.readNextStartElement())
                {
                    if(xml.name() == "Section")
                    {
                        cnt += SectionPropertyEditor::getHtml(&xml);
                        xml.readNext();
                    }
                    else
                    {
                        xml.skipCurrentElement();
                    }
                }
            }
        }
        file.close();

        pluginvars.clear();
        foreach(QString key, Plugins::elementPluginNames())
        {
            if(Plugins::isPluginUsed(key))
            {
                ElementEditorInterface *editor = Plugins::getElementPlugin(key);
                pluginvars["styles"] = pluginvars["styles"].toString() + editor->pluginStyles();
                pluginvars["scripts"] = pluginvars["scripts"].toString() + editor->pluginScripts();
                editor->installAssets(m_sitesPath + "/" + m_site->title() + "/assets");
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
        qWarning() << "Generator::generateSite(): Unable to open file " + m_site->sourcePath() + "/" + subdir + "/" + content->source();
}

QString Generator::translateTemplate(QString layout, Mode mode)
{
    QString rc = "";
    QString path = m_site->sourcePath() + (mode == Layout ? "/layouts/" : "/includes/") + layout;

    QFile site(path);
    if(!site.exists())
        path = m_themePath + "/" + m_site->theme() + (mode == Layout ? "/layouts/" : "/includes/") + layout;

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QString content = QString::fromUtf8(file.readAll());
        file.close();

        int pos = content.indexOf("{% include ");
        int end = 0;
        if(pos >= 0)
        {
            while(pos >= 0)
            {
                QString rest = content.mid(end, pos - end);
                rc += rest;
                end = content.indexOf("%}", pos + 11) + 2;
                QString include = content.mid(pos + 11, end - pos - 13).trimmed();
                QString trans = translateTemplate(include, Include);
                rc += trans;
                pos = content.indexOf("{% include ", end + 1);
                if(content.at(end) == "\n")
                    end++;
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

bool Generator::nextTokens(QString content, QStringList tokens)
{
    int pos = 0;
    int len = content.length();
    int token = 0;
    while(pos < len)
    {
        QChar ch = content.at(pos);
        if(ch == " ")
            pos++;
        else
        {
            if(content.mid(pos).startsWith(tokens.at(token)) && true)
            {
                pos += tokens.at(token).length();
                token++;
                if(token == tokens.count())
                    return true;
            }
            else
                return false;
        }
    }
    return false;
}

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
                        else if(nextTokens(content.mid(pos), QStringList() << "{%" << "else"  << "%}"))
                        {
                            if(ifvars->m_isTrue && ifvars->m_inIf)
                                rc += ifvars->m_ifContent;
                            ifvars->m_inIf = false;
                            ifvars->m_inElse = true;
                            pos = content.indexOf("%}", pos) + 2;
                            if(pos < len && content.at(pos) == "\n")
                                pos++;
                            break;
                        }
                        else if(nextTokens(content.mid(pos), QStringList() << "{%" << "endif" << "%}"))
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
                            pos = content.indexOf("%}", pos) + 2;
                            if(pos < len && content.at(pos) == "\n")
                                pos++;
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
                    if(content.at(pos) == "\n")
                        pos++;
                }
                else if(exp.startsWith("if "))
                {
                    // found nested if statements so we push the ifvars on the stack
                    if(ifvars->m_inIf)
                    {
                        if(ifvars->m_isTrue)
                        {
                            rc += ifvars->m_ifContent;
                            ifvars->m_ifContent = "";
                            stack.push(ifvars);
                            ifvars = new IfVars;
                        }
                        else
                        {
                            // skip to endif
                            while (pos < len)
                            {
                                if(nextTokens(content.mid(pos), QStringList() << "{%" << "endif" << "%}"))
                                {
                                    pos = content.indexOf("%}", pos) + 2;
                                    if(pos < len && content.at(pos) == "\n")
                                        pos++;
                                    break;
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
                                if(nextTokens(content.mid(pos), QStringList() << "{%" << "endif" << "%}"))
                                {
                                    pos = content.indexOf("%}", pos) + 2;
                                    if(pos < len && content.at(pos) == "\n")
                                        pos++;
                                    break;
                                }
                                else
                                    pos++;
                            }
                            break;
                        }
                        else
                        {
                            rc += ifvars->m_elseContent;
                            ifvars->m_elseContent = "";
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
                        if(right == "false")
                            ifvars->m_isTrue = left == "" || left == "false";
                        else
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
                        else // {% if value %}
                        {
                            QString var = exp.mid(2).trimmed();
                            QString value = translateVar(var, loopvars).toString();
                            ifvars->m_isTrue = value == "true";
                        }
                    }
                    ifvars->m_inIf = true;
                    ifvars->m_inElse = false;
                    ifvars->m_ifContent = "";
                    ifvars->m_elseContent = "";

                    if(pos < len && content.at(pos) == "\n")
                        pos++;
                }
                break;
            }
            case InLoop:
            {
                while (pos < len)
                {
                    if(nextTokens(content.mid(pos), QStringList() << "{%" << "for "))
                    {
                        // found nested for
                        loopDeep++;
                        loopContent += "{% for ";
                        pos = content.indexOf("for ", pos) + 4;
                    }
                    else if(nextTokens(content.mid(pos), QStringList() << "{%" << "endfor" << "%}"))
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
                            loopContent += content.mid(pos, content.indexOf("%}", pos) - pos + 2);
                        }
                        pos = content.indexOf("%}", pos) + 2;
                        if(pos < len && content.at(pos) == "\n")
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

    if(exp == "true")
        return true;

    if(exp == "false")
        return false;

    int posOpen = exp.indexOf("[");
    if(posOpen > 0)
    {
        int posClose = exp.indexOf("]");
        QString index = exp.mid(posOpen + 1, posClose - posOpen - 1).trimmed();
        QString var = exp.mid(0, posOpen).trimmed();
        indexValue = translateVar(index, loopvars).toString();
        if(indexValue.isEmpty())
            return "";
        exp = var;
    }

    if(exp.startsWith("page."))
    {
        if(pagevars.contains(exp.mid(5)))
            return pagevars.value(exp.mid(5));
        else
            return "";
        //return pagevars[exp.mid(5)];
    }
    else if(exp.startsWith("site."))
    {
        if(indexValue.isEmpty())
        {
            if(sitevars.contains(exp.mid(5)))
                return sitevars.value(exp.mid(5));
            else
                return "";
            //return sitevars[exp.mid(5)];
        }
        else
        {
            if(sitevars.contains(exp.mid(5)))
            {
                QVariantMap map = sitevars.value(exp.mid(5)).toMap();
                if(map.contains(indexValue))
                    return map.value(indexValue);
                else
                    return "";
            }
            else
                return "";
            //return sitevars[exp.mid(5)].toMap()[indexValue];
        }
    }
    else if(exp.startsWith("plugin."))
    {
        if(pluginvars.contains(exp.mid(7)))
            return pluginvars.value(exp.mid(7));
        else
            return "";
        //return pluginvars[exp.mid(7)];
    }
    else if(exp.startsWith("theme."))
    {
        if(themevars.contains(exp.mid(6)))
            return themevars.value(exp.mid(6));
        else
            return "";
    }
    else
    {
        int dot = exp.indexOf(".");
        if(dot <= 0)
        {
            if(pagevars.contains(exp))
                return pagevars.value(exp);
            else
                return "";
            //return pagevars[exp];
        }
        QString var = exp.mid(0, dot);
        QString value = exp.mid(dot + 1);
        QVariant obj = loopvars[var];
        QVariantMap map = obj.toMap();
        if(map.contains(value))
            return map.value(value);
        else
            return "";
        //return map[value];
    }
}

void Generator::copyPath(QString src, QString dst)
{
    QDir dir(src);
    if (!dir.exists())
        return;

    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString dst_path = dst + QDir::separator() + d;
        dir.mkpath(dst_path);
        copyPath(src + QDir::separator() + d, dst_path);
    }

    foreach (QString f, dir.entryList(QDir::Files))
    {
        QFile::copy(src + QDir::separator() + f, dst + QDir::separator() + f);
    }
}
