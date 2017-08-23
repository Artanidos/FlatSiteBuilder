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
#include <QTableWidgetItem>
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
#include "commands.h"
#include "animationlabel.h"
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

    initUndoRedo();
    initGui();
    readSettings();
    if(!install())
    {
        loadProject(m_defaultPath + "/Site.xml");
    }
    m_dashboardExpander->setExpanded(true);
    showDashboard();
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
    QWidget *centralWidget = new QWidget();
    m_layout = new QVBoxLayout();
    centralWidget->setLayout(m_layout);
    QMainWindow::setCentralWidget(centralWidget);
    m_panel = 0;

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
    reloadProject();

    // create temp dir for undo redo
    QString tempPath = m_site->path().mid(m_site->path().lastIndexOf("/") + 1);
    QDir temp(QDir::tempPath() + "/FlatSiteBuilder");
    temp.mkdir(tempPath);
    temp.cd(tempPath);
    temp.mkdir("pages");
    temp.mkdir("posts");
}

void MainWindow::reloadProject()
{
    m_site->contents().clear();
    m_site->menus().clear();

    QDomDocument doc;
    QFile file(m_site->path() + "/Site.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open " + m_site->path() + "/Site.xml";
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
}

void MainWindow::projectUpdated(QString text)
{
    QUndoCommand *changeCommand = new ChangeProjectCommand(this, m_site, text);
    m_undoStack->push(changeCommand);
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

void MainWindow::setCentralWidget(QWidget *widget)
{
    if(m_panel)
    {
        m_layout->replaceWidget(m_panel, widget);
        delete m_panel;
    }
    else
        m_layout->addWidget(widget);
    m_panel = widget;
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

void MainWindow::editContent(QTableWidgetItem *item)
{
    Content *content = qvariant_cast<Content*>(item->data(Qt::UserRole));
    m_editor = new ContentEditor(m_site, content);
    m_editor->setUndoStack(m_undoStack);
    connect(m_editor, SIGNAL(contentUpdated(QString)), this, SLOT(projectUpdated(QString)));
    connect(m_editor, SIGNAL(preview(Content*)), this, SLOT(previewSite(Content*)));
    connect(this, SIGNAL(siteLoaded(Site*)), m_editor, SLOT(siteLoaded(Site*)));
    connect(m_editor, SIGNAL(contentEditorClosed(QWidget*)), this, SLOT(contentEditorClosed(QWidget*)));
    connect(m_editor, SIGNAL(contentHasChanged(Content*)), this, SLOT(contentHasChanged(Content*)));
    animateIn(item);
    m_editedItem = item;
}

void MainWindow::contentHasChanged(Content *content)
{
    QTableWidget *table = m_editedItem->tableWidget();
    m_editedItem->setText(content->title());
    QTableWidgetItem *layout = table->item(m_editedItem->row(), 2);
    layout->setText(content->layout());
    QTableWidgetItem *author = table->item(m_editedItem->row(), 3);
    author->setText(content->author());
    QTableWidgetItem *date = table->item(m_editedItem->row(), 4);
    date->setText(content->date().toString("dd.MM.yyyy"));
}

void MainWindow::animateIn(QTableWidgetItem *item)
{
    QTableWidget *list = item->tableWidget();

    // create a cell widget to get the right position in the table
    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel(item->data(Qt::DisplayRole).toString()));
    widget->setLayout(layout);
    list->setCellWidget(item->row(), 1, widget);
    QPoint pos = widget->mapTo(m_panel, QPoint(0,0));

    // make screenprint from cell widget
    QPixmap pixmap(widget->size());
    widget->render(&pixmap);

    // make screenprint from central widget
    QPixmap pixmapScroll(m_panel->size());
    m_panel->render(&pixmapScroll);

    m_animationPanel = new QWidget();
    QLabel *scroll = new QLabel();
    scroll->setPixmap(pixmapScroll);
    scroll->setParent(m_animationPanel);
    scroll->move(0, 0);
    AnimationLabel *anim = new AnimationLabel();
    anim->setPixmap(pixmap);
    anim->setAlignment(Qt::AlignTop);
    anim->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, widget->palette().background().color());
    anim->setPalette(pal);
    anim->setParent(m_animationPanel);
    m_animationPanel->setMinimumWidth(m_panel->size().width());
    m_animationPanel->setMinimumHeight(m_panel->size().height());
    m_layout->replaceWidget(m_panel, m_animationPanel);
    m_panel->hide();

    m_animationgroup = new QParallelAnimationGroup();
    QPropertyAnimation *animx = new QPropertyAnimation();
    animx->setDuration(300);
    animx->setStartValue(pos.x());
    animx->setEndValue(0);
    animx->setTargetObject(anim);
    animx->setPropertyName("x");
    m_animationgroup->addAnimation(animx);
    QPropertyAnimation *animy = new QPropertyAnimation();
    animy->setDuration(300);
    animy->setStartValue(pos.y());
    animy->setEndValue(0);
    animy->setTargetObject(anim);
    animy->setPropertyName("y");
    m_animationgroup->addAnimation(animy);
    QPropertyAnimation *animw = new QPropertyAnimation();
    animw->setDuration(300);
    animw->setStartValue(widget->size().width());
    animw->setEndValue(m_panel->size().width());
    animw->setTargetObject(anim);
    animw->setPropertyName("width");
    m_animationgroup->addAnimation(animw);
    QPropertyAnimation *animh = new QPropertyAnimation();
    animh->setDuration(300);
    animh->setStartValue(widget->size().height());
    animh->setEndValue(m_panel->size().height());
    animh->setTargetObject(anim);
    animh->setPropertyName("height");
    m_animationgroup->addAnimation(animh);
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    m_animationgroup->start();

    list->removeCellWidget(item->row(), 1);
}

void MainWindow::animationFineshedZoomIn()
{
    m_layout->replaceWidget(m_animationPanel, m_editor);
    m_animationPanel->hide();
    //m_panel = m_editor;
}

void MainWindow::animationFineshedZoomOut()
{
    m_layout->replaceWidget(m_animationPanel, m_panel);
    m_animationPanel->hide();
    m_panel->show();
    delete m_animationPanel;
    delete m_animationgroup;
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

void MainWindow::contentEditorClosed(QWidget *w)
{
    m_layout->replaceWidget(w, m_animationPanel);
    m_animationPanel->show();
    delete w;
    m_animationgroup->setDirection(QAbstractAnimation::Backward);
    disconnect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomOut()));
    m_animationgroup->start();
}
