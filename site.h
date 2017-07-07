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
#include "page.h"

class Site : public QObject
{
    Q_OBJECT

public:
    Site();

    QString theme() {return m_theme;}
    void setTheme(QString theme) {m_theme = theme;}

    QString title() {return m_title;}
    void setTitle(QString title) {m_title = title;}

    QList<Page *> pages() {return m_pages;}

private:
    QString m_theme;
    QString m_title;
    QList<Page *> m_pages;
};

#endif // SITE_H
