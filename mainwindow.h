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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTreeWidget>
#include <QGraphicsView>
#include <QUndoStack>
#include "htmlhighlighter.h"
#include "expander.h"
#include "site.h"

class QNetworkReply;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:
    void siteLoaded(Site *site);

private:
    void writeSettings();
    void readSettings();
    void initGui();
    bool install();
    void installFiles(QString sourceDir, QString targetDir, bool readOnly = true);
    void showHtml(QString url);

    Expander *m_dashboardExpander;
    Expander *m_media;
    Expander *m_content;
    Expander *m_appearance;
    Expander *m_plugins;
    Expander *m_settings;
    Site *m_site;
    QString m_defaultPath;
    QUndoStack *m_undoStack;

private slots:
    void dashboardExpanded(bool value);
    void contentExpanded(bool value);
    void mediaExpanded(bool value);
    void apearanceExpanded(bool value);
    void pluginsExpanded(bool value);
    void settingsExpanded(bool value);
    void showDashboard();
    void showPosts();
    void showPages();
    void addPost();
    void addPage();
    void editContent(Content *page);
    void saveProject();
    void loadProject(QString path);
    void previewSite(Content *content);
    void publishSite();
    void createSite();
    void notImplemented();
    void fileIsReady(QNetworkReply *reply);
};

#endif // MAINWINDOW_H
