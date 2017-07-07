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

#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include "section.h"

class Page : public QObject
{
    Q_OBJECT

public:
    Page();

    QString title() {return m_title;}
    void setTitle(QString title) {m_title = title;}

    QString url() {return m_url;}
    void setUrl(QString url) {m_url = url;}

    QString layout() {return m_layout;}
    void setLayout(QString layout) {m_layout = layout;}

    QString author() {return m_author;}
    void setAuthor(QString author) {m_author = author;}

    QList<Section *> sections() {return m_sections;}

    QString getHtml();

private:
    QString m_title;
    QString m_url;
    QString m_layout;
    QString m_author;
    QList<Section *> m_sections;
};

#endif // PAGE_H
