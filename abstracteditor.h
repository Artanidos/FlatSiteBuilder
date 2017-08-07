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

#ifndef ABSTRACTEDITOR_H
#define ABSTRACTEDITOR_H

#include <QWidget>
#include "site.h"

class AbstractEditor : public QWidget
{
    Q_OBJECT

public:
    AbstractEditor();

    void setSite(Site *site) {m_site = site;}
    bool changed() {return m_changed;}
    virtual void setContent(QDomElement){}
    virtual QDomElement content() {return m_element;}

signals:
    void close(QWidget*);

public slots:
    void contentChanged() {m_changed = true;}

protected:
    QDomElement m_element;
    bool m_changed;
    Site *m_site;
};

#endif // ABSTRACTEDITOR_H
