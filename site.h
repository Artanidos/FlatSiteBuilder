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

#ifndef SITE_H
#define SITE_H

#include <QObject>
#include "content.h"
#include "menu.h"

class Site : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)

public:
    Site() {}
    Site(QObject *) {}
    Site(QString filename);

    QString theme() {return m_theme;}
    void setTheme(QString theme) {m_theme = theme;}

    QString themeAccent() {return m_themeAccent;}
    void setThemeAccent(QString accent) {m_themeAccent = accent;}

    QString title() {return m_title;}
    void setTitle(QString title);

    QString author() {return m_author;}
    void setAuthor(QString author) {m_author = author;}

    QString keywords() {return m_keywords;}
    void setKeywords(QString keywords) {m_keywords = keywords;}

    QString description() {return m_description;}
    void setDescription(QString desc) {m_description = desc;}

    QString copyright() {return m_copyright;}
    void setCopyright(QString copyright) {m_copyright = copyright;}

    QList<Content *> contents() {return m_contents;}
    void addContent(Content *content) {m_contents.append(content);}
    void removeContent(Content *content) {m_contents.removeOne(content);}

    QList<Menu *> menus() {return m_menus;}
    void addMenu(Menu *menu) {m_menus.append(menu);}
    void removeMenu(Menu *menu) {m_menus.removeOne(menu);}
    void removeAllMenus() {m_menus.clear();}
    QString sourcePath() {return m_sourcePath;}
    QString deployPath() {return m_deployPath;}

private:
    QString m_filename;
    QString m_sourcePath;
    QString m_deployPath;
    QString m_theme;
    QString m_author;
    QString m_themeAccent;
    QString m_title;
    QString m_description;
    QString m_copyright;
    QString m_keywords;
    QList<Content *> m_contents;
    QList<Menu *> m_menus;
};

#endif // SITE_H
