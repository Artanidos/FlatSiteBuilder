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

#ifndef CONTENT_H
#define CONTENT_H

#include <QObject>
#include <QDate>
#include "section.h"

enum ContentType{Page, Post};

class Content : public QObject
{
    Q_OBJECT

public:
    Content() {}
    Content(ContentType type);

    QString title() {return m_title;}
    void setTitle(QString title) {m_title = title;}

    QString menu() {return m_menu;}
    void setMenu(QString menu) {m_menu = menu;}


    QString source() {return m_source;}
    void setSource(QString source) {m_source = source;}
    QString url()
    {
        QString url = m_source;
        return url.replace(".xml", ".html");
    }

    QString layout() {return m_layout;}
    void setLayout(QString layout) {m_layout = layout;}

    QString author() {return m_author;}
    void setAuthor(QString author) {m_author = author;}

    QDate date() {return m_date;}
    void setDate(QDate date) {m_date = date;}

    QString excerpt() {return m_excerpt;}
    void setExcerpt(QString excerpt) {m_excerpt = excerpt;}

    QList<Section *> sections() {return m_sections;}

    ContentType contentType() {return m_type;}

private:
    QString m_title;
    QString m_source;
    QString m_layout;
    QString m_author;
    QString m_excerpt;
    QString m_menu;
    QList<Section *> m_sections;
    ContentType m_type;
    QDate m_date;
    bool m_published;
};

class ContentWrapper : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    QString url(Content *o) {return o->url();}
    QString title(Content *o) {return o->title();}
    QString menu(Content *o) {return o->menu();}
    QString date(Content *o) {return o->date().toString("dd.MM.yyyy");}
    QString excerpt(Content *o) {return o->excerpt();}
    QString author(Content *o) {return o->author();}
};

#endif // CONTENT_H
