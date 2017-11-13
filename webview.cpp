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

#include "webview.h"
#include <QTimer>
#include <QTest>
#include <QAuthenticator>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
}

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    connect(this, &QWebEngineView::loadProgress, [this](int progress)
    {
        //qDebug() << "loadProgress" << progress;
    });

    connect(this, &QWebEngineView::loadFinished, [this](bool success)
    {
        if (!success)
        {
            qDebug() << "unable to open webpage";
        }
    });

    connect(this, &QWebEngineView::renderProcessTerminated,
            [=](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode)
    {
        const char *status = "";
        switch (termStatus)
        {
            case QWebEnginePage::NormalTerminationStatus:
                status = "(normal exit)";
                break;
            case QWebEnginePage::AbnormalTerminationStatus:
                status = "(abnormal exit)";
                break;
            case QWebEnginePage::CrashedTerminationStatus:
                status = "(crashed)";
                break;
            case QWebEnginePage::KilledTerminationStatus:
                status = "(killed)";
                break;
        }

        qInfo() << "Render process exited with code" << statusCode << status;
        QTimer::singleShot(0, [this] { reload(); });
    });
}

void WebView::setPage(WebPage *page)
{
    //createWebActionTrigger(page,QWebEnginePage::Forward);
    //createWebActionTrigger(page,QWebEnginePage::Back);
    //createWebActionTrigger(page,QWebEnginePage::Reload);
    //createWebActionTrigger(page,QWebEnginePage::Stop);
    QWebEngineView::setPage(page);
}
