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
#include <QTextBrowser>
#include <QProcess>
#include <QDesktopServices>
#include <QNetworkReply>
#include "hyperlink.h"
#include "generator.h"
#include "sitewizard.h"
#include "expander.h"
#include "site.h"
#include "content.h"
#include "menu.h"
#include "dashboard.h"
#include "contentlist.h"
#include "contenteditor.h"

MainWindow::MainWindow()
{
    m_site = NULL;

    initGui();
    readSettings();
    if(!install())
    {
        if(m_defaultPath.isEmpty())
            m_defaultPath = QDir::homePath();
        else
            loadProject(m_defaultPath + "/Site.xml");
    }
    m_dashboardExpander->setExpanded(true);
    showDashboard();
}

bool MainWindow::install()
{
    QDir installDir(QDir::homePath() + "/FlatSiteBuilder");
    if(installDir.exists())
        return false;
    installDir.setPath(QDir::homePath());
    installDir.mkdir("FlatSiteBuilder");
    installDir.cd("FlatSiteBuilder");
    installDir.mkdir("sites");
    installDir.mkdir("sources");
    installDir.mkdir("themes");
    installDir.cd("themes");
    installDir.mkdir("default");
    installDir.cd("default");
    installDir.mkdir("layouts");
    installDir.mkdir("includes");
    installDir.mkdir("assets");
    installDir.cd("assets");
    installDir.mkdir("css");
    installDir.mkdir("fonts");
    installDir.mkdir("js");
    installDir.cdUp();
    installDir.cdUp();
    installDir.cdUp();
    installDir.cd("sources");
    installDir.mkdir("testsite");
    installDir.cd("testsite");
    installDir.mkdir("layouts");
    installDir.mkdir("pages");
    installDir.mkdir("posts");
    installDir.mkdir("assets");
    installDir.cd("assets");
    installDir.mkdir("css");
    installDir.mkdir("fonts");
    installDir.mkdir("js");
    installDir.mkdir("images");

    QString themeDir = QDir::homePath() + "/FlatSiteBuilder/themes/default";
    QString siteDir = QDir::homePath() + "/FlatSiteBuilder/sources/testsite";
    installFiles(":/themes/default/layouts/", themeDir + "/layouts/");
    installFiles(":/themes/default/includes/", themeDir + "/includes/");
    installFiles(":/themes/default/assets/css/", themeDir + "/assets/css/");
    installFiles(":/themes/default/assets/fonts/", themeDir + "/assets/fonts/");
    installFiles(":/themes/default/assets/js/", themeDir + "/assets/js/");

    installFiles(":/testsite/", siteDir + "/", false);
    installFiles(":/testsite/posts/", siteDir + "/posts/", false);
    installFiles(":/testsite/pages/", siteDir + "/pages/", false);
    installFiles(":/testsite/includes/", siteDir + "/includes/", false);
    installFiles(":/testsite/assets/images/", siteDir + "/assets/images/", false);

    m_defaultPath = siteDir;
    loadProject(m_defaultPath + "/Site.xml");

    Generator *gen = new Generator;
    gen->generateSite(m_site);
    delete gen;

    return true;
}

void MainWindow::installFiles(QString sourceDir, QString targetDir, bool readOnly)
{
    QDir layouts(sourceDir);
    foreach(QString source, layouts.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile::copy(sourceDir + source, targetDir + source);
        if(!readOnly)
        {
            QFile file(targetDir + source);
            file.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
        }
        qDebug() << "Installing file " + targetDir + source;
    }
}

void MainWindow::initGui()
{
    m_dashboardExpander = new Expander("Dashboard", ":/images/dashboard_normal.png", ":/images/dashboard_hover.png", ":/images/dashboard_selected.png");
    m_media = new Expander("Media", ":/images/media_normal.png", ":/images/media_hover.png", ":/images/media_selected.png");
    m_content = new Expander("Content", ":/images/pages_normal.png", ":/images/pages_hover.png", ":/images/pages_selected.png");
    m_appearance = new Expander("Appearance", ":/images/appearance_normal.png", ":/images/appearance_hover.png", ":/images/appearance_selected.png");
    m_plugins = new Expander("Plugins", ":/images/plugin_normal.png", ":/images/plugin_hover.png", ":/images/plugin_selected.png");
    m_settings = new Expander("Settings", ":/images/settings_normal.png", ":/images/settings_hover.png", ":/images/settings_selected.png");

    setWindowTitle(QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion());
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_dashboardExpander);
    vbox->addWidget(m_media);
    vbox->addWidget(m_content);
    vbox->addWidget(m_appearance);
    vbox->addWidget(m_plugins);
    vbox->addWidget(m_settings);
    vbox->addStretch();

    QVBoxLayout *contentBox = new QVBoxLayout();
    Hyperlink *pagesButton = new Hyperlink("Pages");
    Hyperlink *postsButton = new Hyperlink("Posts");
    Hyperlink *categoriesButton = new Hyperlink("Categories");
    Hyperlink *tagsButton = new Hyperlink("Tags");
    contentBox->addWidget(pagesButton);
    contentBox->addWidget(postsButton);
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

    connect(pagesButton, SIGNAL(clicked()), this, SLOT(showPages()));
    connect(postsButton, SIGNAL(clicked()), this, SLOT(showPosts()));
    connect(categoriesButton, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(tagsButton, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_dashboardExpander, SIGNAL(clicked()), this, SLOT(showDashboard()));
    connect(m_content, SIGNAL(clicked()), this, SLOT(showPages()));
    connect(m_media, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_appearance, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_plugins, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_settings, SIGNAL(clicked()), this, SLOT(notImplemented()));
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
        qDebug() << "Unable to open " + filename;
        return;
    }
    if (!doc.setContent(&file))
    {
        qDebug() << "Unable to load the Site content from XML";
        file.close();
        return;
    }
    file.close();

    QDomElement site = doc.documentElement();

    m_site->setTheme(site.attribute("theme", ""));
    m_site->setThemeAccent(site.attribute("theme_accent", ""));
    m_site->setTitle(site.attribute("title", ""));
    m_site->setDescription(site.attribute("description", ""));
    m_site->setGithub(site.attribute("github", ""));
    m_site->setCopyright(site.attribute("copyright"));
    m_site->setKeywords(site.attribute("keywords"));

    QDomElement content = site.firstChildElement("Content");
    while(!content.isNull())
    {
        Content *p;
        if(content.attribute("type", "page") == "page")
            p = new Content(ContentType::Page);
        else
        {
            p = new Content(ContentType::Post);
            p->setExcerpt(content.attribute("excerpt", ""));
        }
        p->setTitle(content.attribute("title"));
        p->setSource(content.attribute("source"));
        p->setMenu(content.attribute("menu", "default"));
        p->setLogo(content.attribute("logo", "logo.png"));
        p->setAuthor(content.attribute("author"));
        p->setLayout(content.attribute("layout", "default"));
        p->setKeywords(content.attribute("keywords"));
        p->setDate(QDate::fromString(content.attribute("date"), "dd.MM.yyyy"));
        m_site->addContent(p);
        content = content.nextSiblingElement("Content");
    }

    QDomElement menu = site.firstChildElement("Menu");
    while(!menu.isNull())
    {
        Menu *m = new Menu();
        m->setName(menu.attribute("name"));
        QDomElement menuitem = menu.firstChildElement("Item");
        while(!menuitem.isNull())
        {
            MenuItem *item = new MenuItem();
            item->setTitle(menuitem.attribute("title"));
            item->setUrl(menuitem.attribute("url"));
            m->addMenuitem(item);
            menuitem = menuitem.nextSiblingElement("Item");
        }
        m_site->addMenu(m);
        menu = menu.nextSiblingElement("Menu");
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
    root.setAttribute("theme_accent", m_site->themeAccent());
    root.setAttribute("title", m_site->title());
    root.setAttribute("description", m_site->description());
    root.setAttribute("github", m_site->github());
    root.setAttribute("copyright", m_site->copyright());
    root.setAttribute("keywords", m_site->keywords());
    doc.appendChild(root);
    foreach(Content *content, m_site->contents())
    {
        QDomElement c = doc.createElement("Content");
        c.setAttribute("type", content->contentType() == ContentType::Page ? "page" : "post");
        c.setAttribute("source", content->source());
        c.setAttribute("title", content->title());
        c.setAttribute("menu", content->menu());
        c.setAttribute("logo", content->logo());
        c.setAttribute("author", content->author());
        c.setAttribute("layout", content->layout());
        c.setAttribute("keywords", content->keywords());
        if(content->contentType() == ContentType::Post)
            c.setAttribute("excerpt", content->excerpt());
        c.setAttribute("date", QString(content->date().toString("dd.MM.yyyy")));
        root.appendChild(c);
    }

    foreach(Menu *menu, m_site->menus())
    {
        QDomElement m = doc.createElement("Menu");
        m.setAttribute("name", menu->name());
        foreach(MenuItem *item, menu->items())
        {
            QDomElement i = doc.createElement("Item");
            i.setAttribute("title", item->title());
            i.setAttribute("url", item->url());
            m.appendChild(i);
        }
        root.appendChild(m);
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    Generator *gen = new Generator();
    gen->generateSite(m_site);
    delete gen;
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

void MainWindow::showDashboard()
{
    Dashboard *db = new Dashboard(m_site, m_defaultPath);
    connect(db, SIGNAL(loadSite(QString)), this, SLOT(loadProject(QString)));
    connect(db, SIGNAL(previewSite(Content *)), this, SLOT(previewSite(Content *)));
    connect(db, SIGNAL(publishSite()), this, SLOT(publishSite()));
    connect(db, SIGNAL(createSite()), this, SLOT(createSite()));
    connect(this, SIGNAL(siteLoaded(Site*)), db, SLOT(siteLoaded(Site*)));
    setCentralWidget(db);
}

void MainWindow::showPosts()
{
    ContentList *list = new ContentList(m_site, ContentType::Post);
    connect(list, SIGNAL(addContent()), this, SLOT(addPost()));
    connect(list, SIGNAL(editContent(Content*)), this, SLOT(editContent(Content *)));
    connect(list, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    setCentralWidget(list);
}

void MainWindow::showPages()
{
    ContentList *list = new ContentList(m_site, ContentType::Page);
    connect(list, SIGNAL(addContent()), this, SLOT(addPage()));
    connect(list, SIGNAL(editContent(Content*)), this, SLOT(editContent(Content *)));
    connect(list, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    setCentralWidget(list);
}

void MainWindow::addPost()
{
    ContentEditor *edit = new ContentEditor(m_site, new Content(ContentType::Post));
    connect(edit, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    connect(edit, SIGNAL(preview(Content*)), this, SLOT(previewSite(Content*)));
    setCentralWidget(edit);
}

void MainWindow::addPage()
{
    ContentEditor *edit = new ContentEditor(m_site, new Content(ContentType::Page));
    connect(edit, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    connect(edit, SIGNAL(preview(Content*)), this, SLOT(previewSite(Content*)));
    setCentralWidget(edit);
}

void MainWindow::editContent(Content *content)
{
    ContentEditor *edit = new ContentEditor(m_site, content);
    connect(edit, SIGNAL(contentUpdated()), this, SLOT(saveProject()));
    connect(edit, SIGNAL(preview(Content*)), this, SLOT(previewSite(Content*)));
    setCentralWidget(edit);
}

void MainWindow::previewSite(Content *content)
{
    QString file;
    QString dir = QDir::homePath() + "/FlatSiteBuilder/sites";
    QDir path(dir + "/" + m_site->title());
    if(!content)
        content = m_site->contents().at(0);
    file = content->url();
    QDesktopServices::openUrl(QUrl(path.absoluteFilePath(file)));
}

void MainWindow::publishSite()
{   
    showHtml("https://artanidos.github.io/FlatSiteBuilder/publish.html");
}

void MainWindow::createSite()
{
    SiteWizard *wiz = new SiteWizard();
    connect(wiz, SIGNAL(loadSite(QString)), this, SLOT(loadProject(QString)));
    wiz->show();
}

void MainWindow::notImplemented()
{
    showHtml("https://artanidos.github.io/FlatSiteBuilder/notimpl.html");
}

void MainWindow::showHtml(QString url)
{
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileIsReady(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::fileIsReady(QNetworkReply *reply)
{
    QTextBrowser *browser = new QTextBrowser();
    setCentralWidget(browser);
    browser->show();
    browser->setHtml(reply->readAll());
}
