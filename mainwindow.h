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
#include "htmlhighlighter.h"
#include "expander.h"
#include "posts.h"
#include "pages.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void writeSettings();
    void readSettings();
    void initPython();
    void initGui();
    void loadProject();

    QTextEdit *editor;
    HtmlHighlighter *highlighter;
    QTreeWidget *treeview;
    QTreeWidgetItem *root;
    Expander *m_dashboard;
    Expander *m_media;
    Expander *m_content;
    Expander *m_appearance;
    Expander *m_plugins;
    Expander *m_settings;
    Posts *m_postsDialog;
    Pages *m_pagesDialog;

private slots:
    void OnPythonQtStdOut(QString str);
    void OnPythonQtStdErr(QString str);
    void dashboardExpanded(bool value);
    void contentExpanded(bool value);
    void mediaExpanded(bool value);
    void apearanceExpanded(bool value);
    void pluginsExpanded(bool value);
    void settingsExpanded(bool value);
    void showDashboard();
    void showPosts();
    void showPages();
};

#endif // MAINWINDOW_H
