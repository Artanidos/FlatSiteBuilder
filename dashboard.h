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
#include <QUrl>
#include "site.h"

class FlatButton;
class QTextBrowser;
class QNetworkReply;
class Dashboard : public QWidget
{
    Q_OBJECT

public:
    Dashboard(Site *site, QString defaultPath);

signals:
    void loadSite(QString filename);
    void createSite();
    void previewSite(Content *);
    void publishSite();
    void buildSite();

private slots:
    void buildClicked();
    void loadClicked();
    void createClicked();
    void publishClicked();
    void previewClicked();
    void siteLoaded(Site *site);
    void fileIsReady(QNetworkReply *reply);
    void anchorClicked(QUrl);

private:
    FlatButton *m_loadButton;
    FlatButton *m_createButton;
    FlatButton *m_publishButton;
    FlatButton *m_previewButton;
    FlatButton *m_buildButton;
    QLabel *m_info;
    Site *m_site;
    QString m_defaultPath;
    QTextBrowser *m_browser;
};

#endif // DASHBOARD_H
