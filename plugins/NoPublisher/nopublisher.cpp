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

#include "nopublisher.h"
#include <QVBoxLayout>
#include <QNetworkReply>
#include <QTextBrowser>
#include <QDesktopServices>
#include <QUrl>

/*
 * Just a sample plugin to demonstrate how to build a publishing plugin
 * Normally you should build a user interface with fields for some properties
 * and a publish button which triggers the process to deploy the html files
 * and assets to the server
 */
NoPublisher::NoPublisher()
{
    QString url = "https://artanidos.github.io/FlatSiteBuilder/publish.html";
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));

    m_browser = new QTextBrowser;
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_browser);
    setLayout(layout);
}

void NoPublisher::setSitePath(QString path)
{
    Q_UNUSED(path)
    // all html files and assets have been copied to this (path) directory
    // normally ~/FlatSiteBuilder/YourSiteName
    // you should copy them to your desired location or push them to github or whereever
}

void NoPublisher::fileIsReady(QNetworkReply *reply)
{
    m_browser->setHtml(reply->readAll());
    connect(m_browser, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));
}

void NoPublisher::anchorClicked(QUrl url)
{
    QDesktopServices::openUrl(url);
}
