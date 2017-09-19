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
#include "interfaces.h"
#include "themechooser.h"
#include <QCloseEvent>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QDir>
#include <QSplitter>
#include <QHeaderView>
#include <QPluginLoader>
#include <QMenu>
#include <QToolBox>
#include <QPalette>
#include <QAction>
#include <QDockWidget>
#include <QDate>
#include <QTextBrowser>
#include <QProcess>
#include <QDesktopServices>
#include <QNetworkReply>
#include <QStatusBar>
#include <QXmlStreamWriter>
#include "hyperlink.h"
#include "generator.h"
#include "commands.h"
#include "sitewizard.h"
#include "expander.h"
#include "site.h"
#include "content.h"
#include "menu.h"
#include "dashboard.h"
#include "contentlist.h"
#include "contenteditor.h"
#include "plugins.h"
#include "menulist.h"
#include "menueditor.h"

MainWindow::MainWindow()
{
    m_site = NULL;
    m_editor = NULL;

    initUndoRedo();
    initGui();
    loadPlugins();
    readSettings();
    if(!install())
    {
        if(!m_defaultPath.isEmpty())
            loadProject(m_defaultPath + "/Site.xml");
    }
    m_dashboardExpander->setExpanded(true);
    showDashboard();
    statusBar()->showMessage("Ready");
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir(QDir::homePath() + "/FlatSiteBuilder/plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
            if(iEditor)
            {
                Plugins::insert(iEditor->className(), iEditor);
                qDebug() << "Plugin loaded" << fileName;
            }
        }
        else
        {
            qDebug() << "Plugin could not ne loaded" << fileName << loader.errorString();
        }
    }
}

void MainWindow::initUndoRedo()
{
    m_undoStack = new QUndoStack;
    QDir temp(QDir::tempPath() + "/FlatSiteBuilder");
    if(temp.exists())
        temp.removeRecursively();
    temp.setPath(QDir::tempPath());
    temp.mkdir("FlatSiteBuilder");
}

bool MainWindow::install()
{
    statusBar()->showMessage("Installing themes and demosite");
    QDir installDir(QDir::homePath() + "/FlatSiteBuilder");
    if(installDir.exists())
        return false;
    installDir.setPath(QDir::homePath());
    installDir.mkdir("FlatSiteBuilder");
    installDir.cd("FlatSiteBuilder");
    installDir.mkdir("sites");
    installDir.mkdir("sources");
    installDir.mkdir("themes");
    installDir.mkdir("plugins");
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
    installDir.mkdir("himu");
    installDir.cd("himu");
    installDir.mkdir("layouts");
    installDir.mkdir("includes");
    installDir.mkdir("assets");
    installDir.cd("assets");
    installDir.mkdir("css");
    installDir.mkdir("fonts");
    installDir.mkdir("js");
    installDir.mkdir("images");
    installDir.cd("images");
    installDir.mkdir("about-us");
    installDir.mkdir("blog");
    installDir.mkdir("clients");
    installDir.mkdir("ico");
    installDir.mkdir("our-team");
    installDir.mkdir("portfolio");
    installDir.mkdir("services");
    installDir.mkdir("slider");
    installDir.mkdir("prettyPhoto");
    installDir.cd("prettyPhoto");
    installDir.mkdir("dark_rounded");
    installDir.mkdir("dark_square");
    installDir.mkdir("default");
    installDir.mkdir("facebook");
    installDir.mkdir("light_rounded");
    installDir.mkdir("light_square");
    installDir.cdUp();
    installDir.cdUp();
    installDir.cdUp();
    installDir.cdUp();
    installDir.cdUp();
    installDir.cd("sources");
    installDir.mkdir("testsite");
    installDir.cd("testsite");
    installDir.mkdir("layouts");
    installDir.mkdir("includes");
    installDir.mkdir("pages");
    installDir.mkdir("posts");
    installDir.mkdir("assets");
    installDir.cd("assets");
    installDir.mkdir("css");
    installDir.mkdir("fonts");
    installDir.mkdir("js");
    installDir.mkdir("images");

    QString themeDir = QDir::homePath() + "/FlatSiteBuilder/themes/default";
    installFiles(":/themes/default/layouts/", themeDir + "/layouts/");
    installFiles(":/themes/default/includes/", themeDir + "/includes/");
    installFiles(":/themes/default/assets/css/", themeDir + "/assets/css/");
    installFiles(":/themes/default/assets/fonts/", themeDir + "/assets/fonts/");
    installFiles(":/themes/default/assets/js/", themeDir + "/assets/js/");

    themeDir = QDir::homePath() + "/FlatSiteBuilder/themes/himu";
    installFiles(":/themes/himu/", themeDir + "/");
    installFiles(":/themes/himu/layouts/", themeDir + "/layouts/");
    installFiles(":/themes/himu/includes/", themeDir + "/includes/");
    installFiles(":/themes/himu/assets/css/", themeDir + "/assets/css/");
    installFiles(":/themes/himu/assets/fonts/", themeDir + "/assets/fonts/");
    installFiles(":/themes/himu/assets/js/", themeDir + "/assets/js/");
    installFiles(":/themes/himu/assets/images/", themeDir + "/assets/images/");
    installFiles(":/themes/himu/assets/images/about-us/", themeDir + "/assets/images/about-us/");
    installFiles(":/themes/himu/assets/images/blog/", themeDir + "/assets/images/blog/");
    installFiles(":/themes/himu/assets/images/clients/", themeDir + "/assets/images/clients/");
    installFiles(":/themes/himu/assets/images/ico/", themeDir + "/assets/images/ico/");
    installFiles(":/themes/himu/assets/images/our-team/", themeDir + "/assets/images/our-team/");
    installFiles(":/themes/himu/assets/images/portfolio/", themeDir + "/assets/images/portfolio/");
    installFiles(":/themes/himu/assets/images/blog/", themeDir + "/assets/images/blog/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/", themeDir + "/assets/images/prettyPhoto/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/dark_rounded/", themeDir + "/assets/images/prettyPhoto/dark_rounded/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/dark_square/", themeDir + "/assets/images/prettyPhoto/dark_square/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/default/", themeDir + "/assets/images/prettyPhoto/default/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/facebook/", themeDir + "/assets/images/prettyPhoto/facebook/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/light_rounded/", themeDir + "/assets/images/prettyPhoto/light_rounded/");
    installFiles(":/themes/himu/assets/images/prettyPhoto/light_square/", themeDir + "/assets/images/prettyPhoto/light_square/");
    installFiles(":/themes/himu/assets/images/services/", themeDir + "/assets/images/services/");
    installFiles(":/themes/himu/assets/images/slider/", themeDir + "/assets/images/slider/");


    QString siteDir = QDir::homePath() + "/FlatSiteBuilder/sources/testsite";
    installFiles(":/testsite/", siteDir + "/", false);
    installFiles(":/testsite/pages/", siteDir + "/pages/", false);

    m_defaultPath = siteDir;
    loadProject(m_defaultPath + "/Site.xml");

    Generator gen;
    gen.generateSite(m_site);

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
        statusBar()->showMessage("Installing file " + targetDir + source);
    }
}

void MainWindow::initGui()
{
    installEventFilter(this);
    m_dashboardExpander = new Expander("Dashboard", ":/images/dashboard_normal.png", ":/images/dashboard_hover.png", ":/images/dashboard_selected.png");
    //m_media = new Expander("Media", ":/images/media_normal.png", ":/images/media_hover.png", ":/images/media_selected.png");
    m_content = new Expander("Content", ":/images/pages_normal.png", ":/images/pages_hover.png", ":/images/pages_selected.png");
    m_appearance = new Expander("Appearance", ":/images/appearance_normal.png", ":/images/appearance_hover.png", ":/images/appearance_selected.png");
    m_plugins = new Expander("Plugins", ":/images/plugin_normal.png", ":/images/plugin_hover.png", ":/images/plugin_selected.png");
    m_settings = new Expander("Settings", ":/images/settings_normal.png", ":/images/settings_hover.png", ":/images/settings_selected.png");

    setWindowTitle(QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion());
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_dashboardExpander);
    //vbox->addWidget(m_media);
    vbox->addWidget(m_content);
    vbox->addWidget(m_appearance);
    vbox->addWidget(m_plugins);
    vbox->addWidget(m_settings);
    vbox->addStretch();

    QVBoxLayout *contentBox = new QVBoxLayout();
    Hyperlink *pagesButton = new Hyperlink("Pages");
    Hyperlink *postsButton = new Hyperlink("Posts");
    //Hyperlink *categoriesButton = new Hyperlink("Categories");
    //Hyperlink *tagsButton = new Hyperlink("Tags");
    contentBox->addWidget(pagesButton);
    contentBox->addWidget(postsButton);
    //contentBox->addWidget(categoriesButton);
    //contentBox->addWidget(tagsButton);
    m_content->addLayout(contentBox);

    QVBoxLayout *appBox = new QVBoxLayout();
    Hyperlink *themesButton = new Hyperlink("Themes");
    //Hyperlink *widgetsButton = new Hyperlink("Widgets");
    Hyperlink *menusButton = new Hyperlink("Menus");
    appBox->addWidget(menusButton);
    appBox->addWidget(themesButton);
    //appBox->addWidget(widgetsButton);

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
    //connect(m_media, SIGNAL(expanded(bool)), this, SLOT(mediaExpanded(bool)));
    connect(m_content, SIGNAL(expanded(bool)), this, SLOT(contentExpanded(bool)));
    connect(m_appearance, SIGNAL(expanded(bool)), this, SLOT(apearanceExpanded(bool)));
    connect(m_plugins, SIGNAL(expanded(bool)), this, SLOT(pluginsExpanded(bool)));
    connect(m_settings, SIGNAL(expanded(bool)), this, SLOT(settingsExpanded(bool)));
    connect(menusButton, SIGNAL(clicked()), this, SLOT(showMenus()));
    connect(pagesButton, SIGNAL(clicked()), this, SLOT(showPages()));
    connect(postsButton, SIGNAL(clicked()), this, SLOT(showPosts()));
    //connect(categoriesButton, SIGNAL(clicked()), this, SLOT(notImplemented()));
    //connect(tagsButton, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_dashboardExpander, SIGNAL(clicked()), this, SLOT(showDashboard()));
    connect(m_content, SIGNAL(clicked()), this, SLOT(showPages()));
    //connect(m_media, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_appearance, SIGNAL(clicked()), this, SLOT(showMenus()));
    connect(themesButton, SIGNAL(clicked()), this, SLOT(showThemes()));
    connect(m_plugins, SIGNAL(clicked()), this, SLOT(notImplemented()));
    connect(m_settings, SIGNAL(clicked()), this, SLOT(notImplemented()));
}

void MainWindow::dashboardExpanded(bool value)
{
    if(value)
    {
        //m_media->setExpanded(false);
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
        //m_media->setExpanded(false);
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
        //m_media->setExpanded(false);
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
        //m_media->setExpanded(false);
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
        //m_media->setExpanded(false);
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
        m_plugins->setExpanded(false);
    }
}

void MainWindow::loadProject(QString filename)
{
    m_site = new Site(filename);
    reloadProject();

    // create temp dir for undo redo
    QString tempPath = m_site->sourcePath().mid(m_site->sourcePath().lastIndexOf("/") + 1);
    QDir temp(QDir::tempPath() + "/FlatSiteBuilder");
    temp.mkdir(tempPath);
    temp.cd(tempPath);
    temp.mkdir("pages");
    temp.mkdir("posts");
}

void MainWindow::reloadProject()
{
    m_site->contents().clear();

    QFile file(m_site->sourcePath() + "/Site.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        statusBar()->showMessage("Unable to open " + m_site->sourcePath() + "/Site.xml");
        return;
    }
    QXmlStreamReader xml(&file);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "Site")
        {
            m_site->setTheme(xml.attributes().value("theme").toString());
            m_site->setThemeAccent(xml.attributes().value("theme_accent").toString());
            m_site->setTitle(xml.attributes().value("title").toString());
            m_site->setDescription(xml.attributes().value("description").toString());
            m_site->setCopyright(xml.attributes().value("copyright").toString());
            m_site->setKeywords(xml.attributes().value("keywords").toString());
            m_site->setAuthor(xml.attributes().value("author").toString());
        }
    }
    file.close();

    QDir dir(m_site->sourcePath() + "/pages");
    foreach(QString filename, dir.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile pagefile(m_site->sourcePath() + "/pages/" + filename);
        if (!pagefile.open(QIODevice::ReadOnly))
        {
            statusBar()->showMessage("Unable to open " + filename);
            return;
        }

        QXmlStreamReader stream(&pagefile);
        if(stream.readNextStartElement())
        {
            if(stream.name() == "Content")
            {
                Content *content = new Content(ContentType::Page);
                content->setTitle(stream.attributes().value("title").toString());
                content->setAuthor(stream.attributes().value("author").toString());
                content->setLayout(stream.attributes().value("layout").toString());
                content->setLogo(stream.attributes().value("logo").toString());
                content->setKeywords(stream.attributes().value("keywords").toString());
                content->setMenu(stream.attributes().value("menu").toString());
                content->setDate(QDate::fromString(stream.attributes().value("date").toString(), "dd.MM.yyyy"));
                content->setSource(filename);
                m_site->addContent(content);
            }
        }
        pagefile.close();
    }

    QDir posts(m_site->sourcePath() + "/posts");
    foreach(QString filename, posts.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile postfile(m_site->sourcePath() + "/posts/" + filename);
        if (!postfile.open(QIODevice::ReadOnly))
        {
            statusBar()->showMessage("Unable to open " + filename);
            return;
        }

        QXmlStreamReader stream(&postfile);
        if(stream.readNextStartElement())
        {
            if(stream.name() == "Content")
            {
                Content *content = new Content(ContentType::Post);
                content->setTitle(stream.attributes().value("title").toString());
                content->setAuthor(stream.attributes().value("author").toString());
                content->setLayout(stream.attributes().value("layout").toString());
                content->setLogo(stream.attributes().value("logo").toString());
                content->setExcerpt(stream.attributes().value("excerpt").toString());
                content->setKeywords(stream.attributes().value("keywords").toString());
                content->setMenu(stream.attributes().value("menu").toString());
                content->setDate(QDate::fromString(stream.attributes().value("date").toString(), "dd.MM.yyyy"));
                content->setSource(filename);
                m_site->addContent(content);
            }
        }
        postfile.close();
    }

    QFile menufile(m_site->sourcePath() + "/Menus.xml");
    if (!menufile.open(QIODevice::ReadOnly))
    {
        statusBar()->showMessage("Unable to open " + m_site->sourcePath() + "/Menus.xml");
        return;
    }
    int id = 0;
    QXmlStreamReader menu(&menufile);
    if(menu.readNextStartElement())
    {
        if(menu.name() == "Menus")
        {
            while(menu.readNextStartElement())
            {
                if(menu.name() == "Menu")
                {
                    Menu *m = new Menu();
                    m->setId(id++);
                    m->setName(menu.attributes().value("name").toString());
                    while(menu.readNextStartElement())
                    {
                        if(menu.name() == "Item")
                        {
                            MenuItem *item = new MenuItem();
                            item->setTitle(menu.attributes().value("title").toString());
                            item->setUrl(menu.attributes().value("url").toString());
                            item->setIcon(menu.attributes().value("icon").toString());

                            while(menu.readNextStartElement())
                            {
                                if(menu.name() == "Item")
                                {
                                    MenuItem *subitem = new MenuItem();
                                    subitem->setSubitem(true);
                                    subitem->setTitle(menu.attributes().value("title").toString());
                                    subitem->setUrl(menu.attributes().value("url").toString());
                                    subitem->setIcon(menu.attributes().value("icon").toString());
                                    item->addMenuitem(subitem);
                                    menu.readNext();
                                }
                                else
                                    menu.skipCurrentElement();

                            }
                            m->addMenuitem(item);
                            menu.readNext();
                        }
                        else
                            menu.skipCurrentElement();
                    }
                    m_site->addMenu(m);
                }
                else
                    menu.skipCurrentElement();
            }
        }
    }
    menufile.close();

    emit siteLoaded(m_site);

    statusBar()->showMessage(m_site->title() + " has been loaded");
}

void MainWindow::saveProject()
{
    QFile file(m_site->sourcePath() + "/Site.xml");
    if(!file.open(QFile::WriteOnly))
    {
        statusBar()->showMessage("Unable to open file " + m_site->sourcePath() + "/Site.xml");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Site");
    xml.writeAttribute("theme", m_site->theme());
    xml.writeAttribute("theme_accent", m_site->themeAccent());
    xml.writeAttribute("title", m_site->title());
    xml.writeAttribute("description", m_site->description());
    xml.writeAttribute("copyright", m_site->copyright());
    xml.writeAttribute("keywords", m_site->keywords());
    xml.writeAttribute("author", m_site->author());

    foreach(Menu *menu, m_site->menus())
    {
        xml.writeStartElement("Menu");
        xml.writeAttribute("name", menu->name());
        foreach(MenuItem *item, menu->items())
        {
            xml.writeStartElement("Item");
            xml.writeAttribute("title", item->title());
            xml.writeAttribute("url", item->url());
            xml.writeAttribute("icon", item->icon());
            foreach(MenuItem *subitem, item->items())
            {
                xml.writeStartElement("Item");
                xml.writeAttribute("title", subitem->title());
                xml.writeAttribute("url", subitem->url());
                xml.writeAttribute("icon", subitem->icon());
                xml.writeEndElement();
            }
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

    statusBar()->showMessage(m_site->title() + " has been saved");
}

void MainWindow::projectUpdated(QString text)
{
    QUndoCommand *changeCommand = new ChangeProjectCommand(this, m_site, text);
    m_undoStack->push(changeCommand);
    statusBar()->showMessage("The project should be rebuildet.");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,  QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    if(m_site)
        settings.setValue("lastSite", m_site->sourcePath());
}

void MainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,  QCoreApplication::organizationName(), QCoreApplication::applicationName());
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
    connect(db, SIGNAL(buildSite()), this, SLOT(buildSite()));
    connect(this, SIGNAL(siteLoaded(Site*)), db, SLOT(siteLoaded(Site*)));
    setCentralWidget(db);
}

void MainWindow::showPosts()
{
    ContentList *list = new ContentList(m_site, ContentType::Post);
    connect(list, SIGNAL(editContent(QTableWidgetItem*)), this, SLOT(editContent(QTableWidgetItem *)));
    connect(list, SIGNAL(contentUpdated(QString)), this, SLOT(projectUpdated(QString)));
    setCentralWidget(list);
}

void MainWindow::showPages()
{
    ContentList *list = new ContentList(m_site, ContentType::Page);
    connect(list, SIGNAL(editContent(QTableWidgetItem*)), this, SLOT(editContent(QTableWidgetItem*)));
    connect(list, SIGNAL(contentUpdated(QString)), this, SLOT(projectUpdated(QString)));
    setCentralWidget(list);
}

void MainWindow::showMenus()
{
    MenuList *edit = new MenuList(this, m_site);
    connect(edit, SIGNAL(editContent(QTableWidgetItem*)), this, SLOT(editMenu(QTableWidgetItem*)));
    setCentralWidget(edit);
}

void MainWindow::showThemes()
{
    ThemeChooser *tc = new ThemeChooser(m_site);
    connect(tc, SIGNAL(contentUpdated(QString)), this, SLOT(projectUpdated(QString)));
    setCentralWidget(tc);
}

void MainWindow::editContent(QTableWidgetItem *item)
{
    Content *content = qvariant_cast<Content*>(item->data(Qt::UserRole));
    m_editor = new ContentEditor(this, m_site, content);

    connect(m_editor, SIGNAL(preview(Content*)), this, SLOT(previewSite(Content*)));
    connect(this, SIGNAL(siteLoaded(Site*)), m_editor, SLOT(siteLoaded(Site*)));
    connect(m_editor, SIGNAL(close()), this, SLOT(editorClosed()));
    connect(m_editor, SIGNAL(contentChanged(Content*)), this, SLOT(contentChanged(Content*)));
    animate(item);
}

void MainWindow::editMenu(QTableWidgetItem *item)
{
    Menu *menu = qvariant_cast<Menu*>(item->data(Qt::UserRole));
    MenuEditor *me = new MenuEditor(this, menu, m_site);
    m_editor = me;
    MenuList *list = dynamic_cast<MenuList*>(centralWidget());
    if(list)
    {
        list->registerMenuEditor(me);
        connect(list, SIGNAL(editedItemChanged(QTableWidgetItem*)), this, SLOT(editedItemChanged(QTableWidgetItem*)));
    }
    connect(m_editor, SIGNAL(close()), this, SLOT(editorClosed()));
    connect(m_editor, SIGNAL(contentChanged(Menu*)), this, SLOT(menuChanged(Menu*)));
    animate(item);
}

void MainWindow::editedItemChanged(QTableWidgetItem *item)
{
    // this will happen, if the MenuList.reloadMenu() has been called by the undo.command
    m_list = item->tableWidget();
    m_row = item->row();
    m_cellWidget = new QWidget();
    m_list->setCellWidget(m_row, 1, m_cellWidget);
}

void MainWindow::animate(QTableWidgetItem *item)
{   
    QWidget *panel = centralWidget();
    m_list = item->tableWidget();
    m_row = item->row();

    // create a cell widget to get the right position in the table
    m_cellWidget = new QWidget();
    m_list->setCellWidget(m_row, 1, m_cellWidget);
    QPoint pos = m_cellWidget->mapTo(panel, QPoint(0,0));

    m_editor->setParent(panel);
    m_editor->move(pos);
    m_editor->resize(m_cellWidget->size());
    m_editor->show();

    m_animationgroup = new QParallelAnimationGroup();
    m_animx = new QPropertyAnimation();
    m_animx->setDuration(300);
    m_animx->setStartValue(pos.x());
    m_animx->setEndValue(0);
    m_animx->setTargetObject(m_editor);
    m_animx->setPropertyName("x");
    m_animationgroup->addAnimation(m_animx);
    m_animy = new QPropertyAnimation();
    m_animy->setDuration(300);
    m_animy->setStartValue(pos.y());
    m_animy->setEndValue(0);
    m_animy->setTargetObject(m_editor);
    m_animy->setPropertyName("y");
    m_animationgroup->addAnimation(m_animy);
    m_animw = new QPropertyAnimation();
    m_animw->setDuration(300);
    m_animw->setStartValue(m_cellWidget->size().width());
    m_animw->setEndValue(panel->size().width());
    m_animw->setTargetObject(m_editor);
    m_animw->setPropertyName("width");
    m_animationgroup->addAnimation(m_animw);
    m_animh = new QPropertyAnimation();
    m_animh->setDuration(300);
    m_animh->setStartValue(m_cellWidget->size().height());
    m_animh->setEndValue(panel->size().height());
    m_animh->setTargetObject(m_editor);
    m_animh->setPropertyName("height");
    m_animationgroup->addAnimation(m_animh);
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    m_animationgroup->start();
}

void MainWindow::animationFineshedZoomIn()
{

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

void MainWindow::buildSite()
{
    Generator gen;
    gen.generateSite(m_site);
    statusBar()->showMessage(m_site->title() + " has been generated");
}

void MainWindow::createSite()
{
    SiteWizard *wiz = new SiteWizard();
    connect(wiz, SIGNAL(loadSite(QString)), this, SLOT(loadProject(QString)));
    connect(wiz, SIGNAL(buildSite()), this, SLOT(buildSite()));
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

void MainWindow::anchorClicked(QUrl url)
{
    QDesktopServices::openUrl(url);
}

void MainWindow::fileIsReady(QNetworkReply *reply)
{
    QTextBrowser *browser = new QTextBrowser();
    setCentralWidget(browser);
    browser->show();
    browser->setHtml(reply->readAll());
    connect(browser, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));
}

void MainWindow::editorClosed()
{
    QPoint pos = m_cellWidget->mapTo(centralWidget(), QPoint(0,0));
    // correct end values in case of resizing the window
    m_animx->setStartValue(pos.x());
    m_animy->setStartValue(pos.y());
    m_animw->setStartValue(m_cellWidget->size().width());
    m_animh->setStartValue(m_cellWidget->size().height());
    m_animationgroup->setDirection(QAbstractAnimation::Backward);
    disconnect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomOut()));
    m_animationgroup->start();
}

void MainWindow::animationFineshedZoomOut()
{
    m_list->removeCellWidget(m_row, 1);
    delete m_animationgroup;

    // in the case m_editor was a MenuEditor, we have to unregister it in the MenuList
    // should be refactored some day :-)
    MenuList *list = dynamic_cast<MenuList*>(centralWidget());
    if(list)
        list->unregisterMenuEditor();

    delete m_editor;
    m_editor = NULL;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this && event->type() == QEvent::Resize && m_editor)
    {
        m_editor->resize(centralWidget()->size());
    }
    return false;
}

void MainWindow::contentChanged(Content *content)
{
    m_list->item(m_row, 1)->setText(content->title());
    m_list->item(m_row, 2)->setText(content->source());
    m_list->item(m_row, 3)->setText(content->layout());
    m_list->item(m_row, 4)->setText(content->author());
    m_list->item(m_row, 5)->setText(content->date().toString("dd.MM.yyyy"));
}

void MainWindow::menuChanged(Menu *menu)
{
    m_list->item(m_row, 1)->setText(menu->name());
}

