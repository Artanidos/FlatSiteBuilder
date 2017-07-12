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

#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "site.h"

class Dashboard : public QWidget
{
    Q_OBJECT

public:
    Dashboard(Site *site, QString defaultPath);

    bool eventFilter(QObject * watched, QEvent * event);

signals:
    void loadSite(QString filename);

private slots:
    void loadClicked();
    void createClicked();
    void publishClicked();
    void siteLoaded(Site *site);

private:
    QPushButton *m_loadButton;
    QPushButton *m_createButton;
    QPushButton *m_publishButton;
    QLabel *m_info;
    Site *m_site;
    QString m_defaultPath;
};

#endif // DASHBOARD_H
