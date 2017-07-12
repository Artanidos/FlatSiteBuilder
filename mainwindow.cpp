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
#include <QTextStream>
#include <QDir>
#include <QSplitter>
#include <QHeaderView>
#include <QToolBox>
#include <QPalette>
#include <QAction>
#include <QDockWidget>
#include <QDomDocument>
#include <QDate>
#include <QDesktopServices>
#include "hyperlink.h"
#include "generator.h"
#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "expander.h"
#include "site.h"
#include "content.h"
#include "dashboard.h"
#include "contentlist.h"
#include "contenteditor.h"

MainWindow::MainWindow()
{
    m_site = NULL;

    initPython();
    initGui();
    readSettings();

    if(m_defaultPath.isEmpty())
        m_defaultPath = QDir::homePath();
    else
    {
        loadProject(m_defaultPath + "/Site.xml");
        Generator *gen = new Generator();
        gen->generateSite(m_site);
    }

    m_dashboardExpander->setExpanded(true);
    showDashboard();
}

void MainWindow::initPython()
{
    PythonQt::init();
    PythonQt_QtAll::init();
    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(OnPythonQtStdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(OnPythonQtStdErr(QString)));
    PythonQt::self()->addSysPath("/home/olaf/.local/lib/python2.7/site-packages/");
    PythonQt::self()->addSysPath("/usr/local/lib/python2.7/dist-packages/");

    PythonQt::self()->registerCPPClass("Content", "", "flatsitebuilder", PythonQtCreateObject<ContentWrapper>);

}

void MainWindow::initGui()
{
    m_dashboardExpander = new Expander("Dashboard", ":/images/dashboard_normal.png", ":/images/dashboard_hover.png", ":/images/dashboard_selected.png");
    m_media = new Expander("Media", ":/images/media_normal.png", ":/images/media_hover.png", ":/images/media_selected.png");
    m_content = new Expander("Content", ":/images/pages_normal.png", ":/images/pages_hover.png", ":/images/pages_selected.png");
    m_appearance = new Expander("Appearance", ":/images/appearance_normal.png", ":/images/appearance_hover.png", ":/images/appearance_selected.png");
    m_plugins = new Expander("Plugins", ":/images/plugin_normal.png", ":/images/plugin_hover.png", ":/images/plugin_selected.png");
    m_settings = new Expander("Settings", ":/images/settings_normal.png", ":/images/settings_hover.png", ":/images/settings_selected.png");

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_dashboardExpander);
    vbox->addWidget(m_media);
    vbox->addWidget(m_content);
    vbox->addWidget(m_appearance);
    vbox->addWidget(m_plugins);
    vbox->addWidget(m_settings);
    vbox->addStretch();

    QVBoxLayout *dashBox = new QVBoxLayout();
    Hyperlink *generateButton = new Hyperlink("Generate Site");
    Hyperlink *homeButton = new Hyperlink("Documentation");
    dashBox->addWidget(generateButton);
    dashBox->addWidget(homeButton);
    m_dashboardExpander->addLayout(dashBox);

    QVBoxLayout *contentBox = new QVBoxLayout();
    Hyperlink *postsButton = new Hyperlink("Posts");
    Hyperlink *pagesButton = new Hyperlink("Pages");
    Hyperlink *categoriesButton = new Hyperlink("Categories");
    Hyperlink *tagsButton = new Hyperlink("Tags");
    contentBox->addWidget(postsButton);
    contentBox->addWidget(pagesButton);
    contentBox->addWidget(categoriesButton);
    contentBox->addWidget(tagsButton);
    m_content->addLayout(contentBox);

    QVBoxLayout *appBox = new QVBoxLayout();
    Hyperlink *themesButton = new Hyperlink("Themes");
    Hyperlink *widgetsButton = new Hyperlink("Widgets");
    Hyperlink *menusButton = new Hyperlink("Menus");
    appBox->addWidget(themesButton);
    appBox->addWidget(widgetsButton);
    appBox->addWidget(menusButton);
    m_appearance->addLayout(appBox);

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

    connect(m_dashboardExpander, SIGNAL(expanded(bool)), this, SLOT(dashboardExpanded(bool)));
    connect(m_media, SIGNAL(expanded(bool)), this, SLOT(mediaExpanded(bool)));
    connect(m_content, SIGNAL(expanded(bool)), this, SLOT(contentExpanded(bool)));
    connect(m_appearance, SIGNAL(expanded(bool)), this, SLOT(apearanceExpanded(bool)));
    connect(m_plugins, SIGNAL(expanded(bool)), this, SLOT(pluginsExpanded(bool)));
    connect(m_settings, SIGNAL(expanded(bool)), this, SLOT(settingsExpanded(bool)));

    connect(pagesButton, SIGNAL(linkActivated(QString)), this, SLOT(showPages()));
    connect(postsButton, SIGNAL(linkActivated(QString)), this, SLOT(showPosts()));
    connect(m_dashboardExpander, SIGNAL(clicked()), this, SLOT(showDashboard()));
    connect(m_content, SIGNAL(clicked()), this, SLOT(showPosts()));
}

void MainWindow::dashboardExpanded(bool value)
{
    if(value)
    {
        m_media->setExpanded(false);
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::mediaExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::contentExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
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
        m_dashboardExpander->setExpanded(false);
        m_media->setExpanded(false);
        m_content->setExpanded(false);
        m_plugins->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::pluginsExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
        m_media->setExpanded(false);
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::settingsExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
        m_media->setExpanded(false);
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
    }
}

void MainWindow::loadProject(QString filename)
{
    m_site = new Site(filename);

    QDomDocument doc;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open Site.xml";
        return;
    }
    if (!doc.setContent(&file))
    {
        qDebug() << "Unable to the Site content from XML";
        file.close();
        return;
    }
    file.close();

    QDomElement site = doc.documentElement();

    m_site->setTheme(site.attribute("theme", "defaultTheme"));
    m_site->setTitle(site.attribute("title", "defaultTitle"));

    QDomElement content = site.firstChildElement("Content");
    while(!content.isNull())
    {
        Content *p;
        if(content.attribute("type", "page") == "page")
            p = new Content(ContentType::Page);
        else
            p = new Content(ContentType::Post);
        p->setTitle(content.attribute("title"));
        p->setSource(content.attribute("source"));
        p->setAuthor(content.attribute("author"));
        p->setLayout(content.attribute("layout", "default"));
        p->setDate(QDate::fromString(content.attribute("date"), "dd.MM.yyyy"));
        m_site->addContent(p);
        content = content.nextSiblingElement("Content");
    }

    emit siteLoaded(m_site);
}

void MainWindow::saveProject()
{
    QDomDocument doc;
    QDomElement root;
    QFile file(m_site->path() + "/Site.xml");
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + m_site->path() + "/Site.xml";
        return;
    }
    root = doc.createElement("Site");
    root.setAttribute("theme", m_site->theme());
    root.setAttribute("title", m_site->title());
    doc.appendChild(root);
    foreach(Content *content, m_site->contents())
    {
        QDomElement c = doc.createElement("Content");
        c.setAttribute("type", content->contentType() == ContentType::Page ? "page" : "post");
        c.setAttribute("source", content->source());
        c.setAttribute("title", content->title());
        c.setAttribute("author", content->author());
        c.setAttribute("layout", content->layout());
        c.setAttribute("date", QString(content->date().toString("dd.MM.yyyy")));
        root.appendChild(c);
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    // TODO: start generate in background
    Generator *gen = new Generator();
    gen->generateSite(m_site);
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
    if(m_site)
        settings.setValue("lastSite", m_site->path());
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
    m_defaultPath = settings.value("lastSite").toString();
}

void MainWindow::OnPythonQtStdOut(QString str)
{
    qDebug() << "Python:" << str;
}

void MainWindow::OnPythonQtStdErr(QString str)
{
    qDebug() << "PythonErr:" << str;
}

void MainWindow::showDashboard()
{
    Dashboard *db = new Dashboard(m_site, m_defaultPath);
    connect(db, SIGNAL(loadSite(QString)), this, SLOT(loadProject(QString)));
    connect(db, SIGNAL(previewSite()), this, SLOT(previewSite()));
    connect(this, SIGNAL(siteLoaded(Site*)), db, SLOT(siteLoaded(Site*)));
    setCentralWidget(db);
}

void MainWindow::showPosts()
{
    ContentList *list = new ContentList(m_site, ContentType::Post);
    connect(list, SIGNAL(addContent()), this, SLOT(addPost()));
    connect(list, SIGNAL(editContent(Content*)), this, SLOT(editContent(Content *)));
    setCentralWidget(list);
}

void MainWindow::showPages()
{
    ContentList *list = new ContentList(m_site, ContentType::Page);
    connect(list, SIGNAL(addContent()), this, SLOT(addPage()));
    connect(list, SIGNAL(editContent(Content*)), this, SLOT(editContent(Content *)));
    setCentralWidget(list);
}

void MainWindow::addPost()
{
    ContentEditor *edit = new ContentEditor(m_site, new Content(ContentType::Post));
    connect(edit, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    setCentralWidget(edit);
}

void MainWindow::addPage()
{
    ContentEditor *edit = new ContentEditor(m_site, new Content(ContentType::Page));
    connect(edit, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    setCentralWidget(edit);
}

void MainWindow::editContent(Content *content)
{
    ContentEditor *edit = new ContentEditor(m_site, content);
    connect(edit, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    setCentralWidget(edit);
}

void MainWindow::previewSite()
{
    QString temp = QDir::tempPath();
    QDir path(temp + "/" + m_site->title());
    QDesktopServices::openUrl(QUrl(path.absoluteFilePath("index.html")));
}
