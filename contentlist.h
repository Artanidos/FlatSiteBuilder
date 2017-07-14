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

#ifndef CONTENTLIST_H
#define CONTENTLIST_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "site.h"

class ContentList  : public QWidget
{
    Q_OBJECT

public:
    ContentList(Site *site, ContentType type);

private slots:
    void buttonClicked();
    void deleteButtonClicked();
    void tableDoubleClicked(int, int);
    void checkStateChanged(bool);

signals:
    void addContent();
    void contentUpdated();
    void editContent(Content *content);

private:
    Site *m_site;
    ContentType m_type;
    QTableWidget *m_list;
    QPushButton *m_deleteButton;
};

#endif // CONTENTLIST_H
