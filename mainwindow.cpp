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

#include "mainwindow.h"

#include <QCloseEvent>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QDir>
#include <QSplitter>
#include <QHeaderView>
#include <QToolBox>
#include <QAction>
#include <QDockWidget>
#include "hyperlink.h"

#include "generator.h"
#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "expander.h"

MainWindow::MainWindow()
{
    initPython();
    initGui();
    readSettings();
    loadProject();

    //Generator gen;
    //gen.generateSite("/home/olaf/SourceCode/FlatSiteBuilder/testsite");
}

void MainWindow::initPython()
{
    PythonQt::init();
    PythonQt_QtAll::init();
    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(OnPythonQtStdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(OnPythonQtStdErr(QString)));
    PythonQt::self()->addSysPath("/home/olaf/.local/lib/python2.7/site-packages/");
    PythonQt::self()->addSysPath("/usr/local/lib/python2.7/dist-packages/");
}

void MainWindow::initGui()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    editor = new QTextEdit;
    editor->setFont(font);

    highlighter = new HtmlHighlighter(editor->document());

    m_dashboard = new Expander("Dashboard", ":/images/dashboard_normal.png", ":/images/dashboard_hover.png", ":/images/dashboard_selected.png");
    m_posts = new Expander("Posts", ":/images/post_normal.png", ":/images/post_hover.png", ":/images/post_selected.png");
    m_media = new Expander("Media", ":/images/media_normal.png", ":/images/media_hover.png", ":/images/media_selected.png");
    m_pages = new Expander("Pages", ":/images/pages_normal.png", ":/images/pages_hover.png", ":/images/pages_selected.png");
    m_appearance = new Expander("Appearance", ":/images/appearance_normal.png", ":/images/appearance_hover.png", ":/images/appearance_selected.png");
    m_plugins = new Expander("Plugins", ":/images/plugin_normal.png", ":/images/plugin_hover.png", ":/images/plugin_selected.png");
    m_settings = new Expander("Settings", ":/images/settings_normal.png", ":/images/settings_hover.png", ":/images/settings_selected.png");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_dashboard);
    vbox->addWidget(m_posts);
    vbox->addWidget(m_media);
    vbox->addWidget(m_pages);
    vbox->addWidget(m_appearance);
    vbox->addWidget(m_plugins);
    vbox->addWidget(m_settings);
    vbox->addStretch();

    QVBoxLayout *dashBox = new QVBoxLayout();
    Hyperlink *homeButton = new Hyperlink("Home");
    Hyperlink *updatesButton = new Hyperlink("Updates");
    dashBox->addWidget(homeButton);
    dashBox->addWidget(updatesButton);
    m_dashboard->addLayout(dashBox);

    QVBoxLayout *postsBox = new QVBoxLayout();
    Hyperlink *allPostsButton = new Hyperlink("All posts");
    Hyperlink *addNewButton = new Hyperlink("Add new");
    Hyperlink *categoriesButton = new Hyperlink("Categories");
    Hyperlink *tagsButton = new Hyperlink("Tags");
    postsBox->addWidget(allPostsButton);
    postsBox->addWidget(addNewButton);
    postsBox->addWidget(categoriesButton);
    postsBox->addWidget(tagsButton);
    m_posts->addLayout(postsBox);

    QWidget *scrollContent = new QWidget();
    scrollContent->setLayout(vbox);
    QScrollArea *scroll = new QScrollArea();
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    scroll->setMaximumWidth(200);
    scroll->setMinimumWidth(200);

    QDockWidget *navigationdock = new QDockWidget(tr("Navigation"), this);
    navigationdock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    navigationdock->setWidget(scroll);
    navigationdock->setObjectName("Navigation");

    addDockWidget(Qt::LeftDockWidgetArea, navigationdock);

    setCentralWidget(editor);

    connect(m_dashboard, SIGNAL(expanded(bool)), this, SLOT(dashboardExpanded(bool)));
    connect(m_posts, SIGNAL(expanded(bool)), this, SLOT(postsExpanded(bool)));
    connect(m_media, SIGNAL(expanded(bool)), this, SLOT(mediaExpanded(bool)));
    connect(m_pages, SIGNAL(expanded(bool)), this, SLOT(pagesExpanded(bool)));
    connect(m_appearance, SIGNAL(expanded(bool)), this, SLOT(apearanceExpanded(bool)));
    connect(m_plugins, SIGNAL(expanded(bool)), this, SLOT(pluginsExpanded(bool)));
    connect(m_settings, SIGNAL(expanded(bool)), this, SLOT(settingsExpanded(bool)));
}

void MainWindow::dashboardExpanded(bool value)
{
    if(value)
    {
        m_posts->setExpanded(false);
        m_media->setExpanded(false);
        m_pages->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::postsExpanded(bool value)
{
    if(value)
    {
        m_dashboard->setExpanded(false);
        m_media->setExpanded(false);
        m_pages->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::mediaExpanded(bool value)
{
    if(value)
    {
        m_dashboard->setExpanded(false);
        m_posts->setExpanded(false);
        m_pages->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::pagesExpanded(bool value)
{
    if(value)
    {
        m_dashboard->setExpanded(false);
        m_posts->setExpanded(false);
        m_media->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::apearanceExpanded(bool value)
{
    if(value)
    {
        m_dashboard->setExpanded(false);
        m_posts->setExpanded(false);
        m_media->setExpanded(false);
        m_pages->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::pluginsExpanded(bool value)
{
    if(value)
    {
        m_dashboard->setExpanded(false);
        m_posts->setExpanded(false);
        m_media->setExpanded(false);
        m_pages->setExpanded(false);
        m_appearance->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::settingsExpanded(bool value)
{
    if(value)
    {
        m_dashboard->setExpanded(false);
        m_posts->setExpanded(false);
        m_media->setExpanded(false);
        m_pages->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
    }
}

void MainWindow::loadProject()
{
    /*
    QStringList filter;
    filter << "*.md";
    filter << "*.html";
    filter << "*.yaml";
    filter << "*.css";
    QDir dir("/home/olaf/SourceCode/FlatSiteBuilder/testsite");

    foreach (QString filename, dir.entryList(filter, QDir::Files))
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, filename);
        root->addChild(item);
    }
    treeview->expandAll();
    */
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
        restoreState(settings.value("state").toByteArray());
    }
}

void MainWindow::OnPythonQtStdOut(QString str)
{
    qDebug() << "Python:" << str;
}

void MainWindow::OnPythonQtStdErr(QString str)
{
    qDebug() << "PythonErr:" << str;
}
