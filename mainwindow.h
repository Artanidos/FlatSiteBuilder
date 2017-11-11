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
#include "expander.h"
#include "site.h"
#include "interfaces.h"
#include "animateableeditor.h"

class QNetworkReply;
class QTableWidgetItem;
class ContentEditor;
class ContentList;
class QTableWidget;
class QWebEngineView;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QString installDirectory);

    bool eventFilter(QObject *watched, QEvent *event);
    void reloadProject();
    void saveProject();
    void setCentralWidget(QWidget *widget);
    void actualThemeChanged(QString themename);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

signals:
    void siteLoaded(Site *site);

private:
    void writeSettings();
    void readSettings();
    void initGui();
    void install();
    void initUndoRedo();
    void installFiles(QString sourceDir, QString targetDir, bool readOnly = true, bool recursive = true);
    void installFile(QString sourceFile, QString targetFile, bool readOnly);
    void animate(QTableWidgetItem *item);
    void loadPlugins();

    Hyperlink *m_themeSettingsButton;
    Expander *m_dashboardExpander;
    Expander *m_media;
    Expander *m_content;
    Expander *m_appearance;
    Expander *m_plugins;
    Expander *m_settings;
    Site *m_site;
    QString m_defaultPath;
    QUndoStack *m_undoStack;
    QParallelAnimationGroup *m_animationgroup;
    QPropertyAnimation *m_animx;
    QPropertyAnimation *m_animy;
    QPropertyAnimation *m_animw;
    QPropertyAnimation *m_animh;
    AnimateableEditor *m_editor;
    QTableWidget *m_list;
    QWidget *m_cellWidget;
    int m_row;
    QString m_installDirectory;
    QWebEngineView *m_webView;
    QString m_methodAfterAnimation;
    Content *m_contentAfterAnimation;
    QDockWidget *m_navigationdock;
    FlatButton *m_showDock;

private slots:
    void dashboardExpanded(bool value);
    void contentExpanded(bool value);
    void mediaExpanded(bool value);
    void apearanceExpanded(bool value);
    void settingsExpanded(bool value);
    void showDashboard();
    void showPosts();
    void showPages();
    void showMenus();
    void showThemes();
    void showSettings();
    void showThemesSettings();
    void editContent(QTableWidgetItem *item);
    void editMenu(QTableWidgetItem *item);
    void editedItemChanged(QTableWidgetItem*);
    void previewSite(Content *content);
    void publishSite();
    void createSite();
    void buildSite();
    void loadProject(QString path);
    void editorClosed();
    void animationFineshedZoomIn();
    void animationFineshedZoomOut();
    void contentChanged(Content *content);
    void menuChanged(Menu *menu);
    void webViewLoadFinished(bool success);
    void showMenu();
    void dockVisibilityChanged(bool visible);
};

#endif // MAINWINDOW_H
