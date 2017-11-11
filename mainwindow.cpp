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
#include "webview.h"
#include "sitesettingseditor.h"
#include "installdialog.h"
#include <QCloseEvent>
#include <QTimer>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QCryptographicHash>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QFileDialog>
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
#include <QStatusBar>
#include <QXmlStreamWriter>
#include <QMessageBox>
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

MainWindow::MainWindow(QString installDirectory)
{
    m_site = NULL;
    m_editor = NULL;
    m_installDirectory = installDirectory;
    m_contentAfterAnimation = NULL;

    Generator::setInstallDirectory(installDirectory);

    initUndoRedo();
    initGui();
    readSettings();
    install();
    loadPlugins();

    if(!m_defaultPath.isEmpty())
    {
        loadProject(m_defaultPath + "/Site.xml");

        // if site has never been generated (after install)
        // generate the site
        QDir site(Generator::sitesPath() + "/" + m_site->title());
        if(!site.exists())
        {
            Generator gen;
            gen.generateSite(this, m_site);
        }
    }
    m_dashboardExpander->setExpanded(true);
    showDashboard();
    statusBar()->showMessage("Ready");
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir(m_installDirectory + "/plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            ElementEditorInterface *iEditor = qobject_cast<ElementEditorInterface *>(plugin);
            if(iEditor)
            {
                AnimateableEditor *ae = dynamic_cast<AnimateableEditor*>(iEditor);
                if(ae)
                {
                    Plugins::insert(iEditor->className(), iEditor);
                    qDebug() << "Plugin loaded" << fileName;
                }
                else
                    qDebug() << "Plugin does not implement AnimateableEditor " << fileName;
            }
            ThemeEditorInterface *iThemeEditor = qobject_cast<ThemeEditorInterface *>(plugin);
            if(iThemeEditor)
            {
                QWidget *w = dynamic_cast<QWidget*>(iThemeEditor);
                if(w)
                {
                    Plugins::insert(iThemeEditor->className(), iThemeEditor);
                    qDebug() << "Plugin loaded" << fileName;
                }
                else
                    qDebug() << "Plugin does not implement QWidget " << fileName;
            }
            PublisherInterface *iPublisher = qobject_cast<PublisherInterface*>(plugin);
            if(iPublisher)
            {
                Plugins::insert(iPublisher->className(), iPublisher);
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

void MainWindow::install()
{
    QDir installDir(m_installDirectory);
    QDir pluginsDir(m_installDirectory + "/plugins");
    QDir themesDir(m_installDirectory + "/themes");
    if(!pluginsDir.exists() || !themesDir.exists())
    {
        qDebug() << "Installing themes, plugins and demosite";
        m_defaultPath = installDir.absolutePath() + "/sources/testsite";
    }

    installFiles(QCoreApplication::applicationDirPath() + "/../../plugins", installDir.absolutePath() + "/plugins", true, false);
    installFiles(QCoreApplication::applicationDirPath() + "/../../testsite", installDir.absolutePath() + "/sources/testsite", false);
    installFiles(QCoreApplication::applicationDirPath() + "/../../themes", installDir.absolutePath() + "/themes");
}

void MainWindow::installFiles(QString sourceDir, QString targetDir, bool readOnly, bool recursive)
{
    QDir dir(sourceDir);
    dir.mkpath(targetDir);
    if(recursive)
    {
        foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        {
            QString dst_path = targetDir + QDir::separator() + d;
            dir.mkpath(dst_path);
            installFiles(sourceDir + QDir::separator() + d, dst_path, readOnly, recursive);
        }
    }
    foreach(QString source, dir.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile target(targetDir + QDir::separator() + source);
        // only override readonly files like plugins or themes

        if(target.exists())
        {
            if(readOnly)
            {
                QCryptographicHash md5genTarget(QCryptographicHash::Md5);
                if(target.open(QFile::ReadOnly))
                {
                    md5genTarget.addData(target.readAll());
                    target.close();
                }
                QFile sourceFile(sourceDir + QDir::separator() + source);
                QCryptographicHash md5genSource(QCryptographicHash::Md5);
                if(sourceFile.open(QFile::ReadOnly))
                {
                    md5genSource.addData(sourceFile.readAll());
                    sourceFile.close();
                }

                if(md5genTarget.result() != md5genSource.result())
                {
                    target.remove();
                    installFile(sourceDir + QDir::separator() + source, targetDir + QDir::separator() + source, readOnly);
                }
            }
        }
        else
        {
            installFile(sourceDir + QDir::separator() + source, targetDir + QDir::separator() + source, readOnly);
        }
    }
}

void MainWindow::installFile(QString sourceFile, QString targetFile, bool readOnly)
{
    qDebug() << "installing file" << targetFile;
    QFile::copy(sourceFile, targetFile);
    QFile target(targetFile);
    if(readOnly)
        target.setPermissions(QFileDevice::ReadOwner);
    else
        target.setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
}

void MainWindow::initGui()
{
    installEventFilter(this);
    m_dashboardExpander = new Expander("Dashboard", ":/images/dashboard_normal.png", ":/images/dashboard_hover.png", ":/images/dashboard_selected.png");
    m_content = new Expander("Content", ":/images/pages_normal.png", ":/images/pages_hover.png", ":/images/pages_selected.png");
    m_appearance = new Expander("Appearance", ":/images/appearance_normal.png", ":/images/appearance_hover.png", ":/images/appearance_selected.png");
    m_settings = new Expander("Settings", ":/images/settings_normal.png", ":/images/settings_hover.png", ":/images/settings_selected.png");

    setWindowTitle(QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion());
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(m_dashboardExpander);
    vbox->addWidget(m_content);
    vbox->addWidget(m_appearance);
    vbox->addWidget(m_settings);
    vbox->addStretch();

    QVBoxLayout *contentBox = new QVBoxLayout();
    Hyperlink *pagesButton = new Hyperlink("Pages");
    Hyperlink *postsButton = new Hyperlink("Posts");
    contentBox->addWidget(pagesButton);
    contentBox->addWidget(postsButton);
    m_content->addLayout(contentBox);

    QVBoxLayout *appBox = new QVBoxLayout();
    Hyperlink *themesButton = new Hyperlink("Themes");
    Hyperlink *menusButton = new Hyperlink("Menus");
    m_themeSettingsButton = new Hyperlink("Theme Settings");
    m_themeSettingsButton->setVisible(false);
    appBox->addWidget(menusButton);
    appBox->addWidget(themesButton);
    appBox->addWidget(m_themeSettingsButton);

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
    connect(m_content, SIGNAL(expanded(bool)), this, SLOT(contentExpanded(bool)));
    connect(m_appearance, SIGNAL(expanded(bool)), this, SLOT(apearanceExpanded(bool)));
    connect(m_settings, SIGNAL(expanded(bool)), this, SLOT(settingsExpanded(bool)));
    connect(menusButton, SIGNAL(clicked()), this, SLOT(showMenus()));
    connect(pagesButton, SIGNAL(clicked()), this, SLOT(showPages()));
    connect(postsButton, SIGNAL(clicked()), this, SLOT(showPosts()));
    connect(m_dashboardExpander, SIGNAL(clicked()), this, SLOT(showDashboard()));
    connect(m_content, SIGNAL(clicked()), this, SLOT(showPages()));
    connect(m_appearance, SIGNAL(clicked()), this, SLOT(showMenus()));
    connect(themesButton, SIGNAL(clicked()), this, SLOT(showThemes()));
    connect(m_themeSettingsButton, SIGNAL(clicked()), this, SLOT(showThemesSettings()));
    connect(m_settings, SIGNAL(clicked()), this, SLOT(showSettings()));
}

void MainWindow::dashboardExpanded(bool value)
{
    if(value)
    {
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
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
        m_settings->setExpanded(false);
    }
}

void MainWindow::contentExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
        m_appearance->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::apearanceExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
        m_content->setExpanded(false);
        m_settings->setExpanded(false);
    }
}

void MainWindow::settingsExpanded(bool value)
{
    if(value)
    {
        m_dashboardExpander->setExpanded(false);
        m_content->setExpanded(false);
        m_appearance->setExpanded(false);
    }
}

void MainWindow::loadProject(QString filename)
{
    m_site = new Site(this, filename);
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
    m_site->load();
    m_site->loadMenus();
    m_site->loadPages();
    m_site->loadPosts();

    m_themeSettingsButton->setVisible(false);
    Plugins::setActualThemeEditorPlugin("");
    foreach(QString key, Plugins::themePluginNames())
    {
        ThemeEditorInterface *tei = Plugins::getThemePlugin(key);
        if(tei)
        {
            if(tei->themeName() == m_site->theme())
            {
                Plugins::setActualThemeEditorPlugin(tei->className());
                m_themeSettingsButton->setVisible(true);
                break;
            }
        }
    }
    if(m_site->publisher().isEmpty())
    {
        if(Plugins::publishPluginNames().count() > 0)
            m_site->setPublisher(Plugins::publishPluginNames().at(0));
    }
    Plugins::setActualPublishPlugin(m_site->publisher());
    emit siteLoaded(m_site);
}

void MainWindow::actualThemeChanged(QString themename)
{
    m_themeSettingsButton->setVisible(false);
    foreach(QString key, Plugins::themePluginNames())
    {
        ThemeEditorInterface *tei = Plugins::getThemePlugin(key);
        if(tei)
        {
            if(tei->themeName() == themename)
            {
                m_themeSettingsButton->setVisible(true);
                break;
            }
        }
    }
}

void MainWindow::saveProject()
{
    m_site->save();
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
    if(m_editor)
    {
        m_methodAfterAnimation = "showDashboard";
        editorClosed();
        return;
    }
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
    if(m_editor)
    {
        m_methodAfterAnimation = "showPosts";
        editorClosed();
        return;
    }
    ContentList *list = new ContentList(m_site, ContentType::Post);
    connect(list, SIGNAL(editContent(QTableWidgetItem*)), this, SLOT(editContent(QTableWidgetItem *)));
    setCentralWidget(list);
}

void MainWindow::showPages()
{
    if(m_editor)
    {
        m_methodAfterAnimation = "showPages";
        editorClosed();
        return;
    }
    ContentList *list = new ContentList(m_site, ContentType::Page);
    connect(list, SIGNAL(editContent(QTableWidgetItem*)), this, SLOT(editContent(QTableWidgetItem*)));
    setCentralWidget(list);
}

void MainWindow::showMenus()
{
    if(m_editor)
    {
        m_methodAfterAnimation = "showMenus";
        editorClosed();
        return;
    }
    MenuList *edit = new MenuList(this, m_site);
    connect(edit, SIGNAL(editContent(QTableWidgetItem*)), this, SLOT(editMenu(QTableWidgetItem*)));
    setCentralWidget(edit);
}

void MainWindow::showThemes()
{
    if(m_editor)
    {
        m_methodAfterAnimation = "showThemes";
        editorClosed();
        return;
    }
    ThemeChooser *tc = new ThemeChooser(this, m_site);
    setCentralWidget(tc);
}

void MainWindow::showThemesSettings()
{
    ThemeEditorInterface *tei = Plugins::getThemePlugin(Plugins::actualThemeEditorPlugin());
    if(tei)
    {
        if(m_editor)
        {
            m_methodAfterAnimation = "showThemesSettings";
            editorClosed();
            return;
        }
        QString path = m_site->sourcePath();
        tei->setWindow(this);
        tei->setSourcePath(path);
        setCentralWidget(dynamic_cast<QWidget*>(tei));
    }
    else
        statusBar()->showMessage("Unable to load plugin " + Plugins::actualThemeEditorPlugin());
}

void MainWindow::showSettings()
{
    if(m_editor)
    {
        m_methodAfterAnimation = "showSettings";
        editorClosed();
        return;
    }
    SiteSettingsEditor *sse = new SiteSettingsEditor(this, m_site);
    setCentralWidget(sse);
}

void MainWindow::setCentralWidget(QWidget *widget)
{
    // do not delete plugin editors
    QWidget *oldWidget = takeCentralWidget();
    ThemeEditorInterface *tei = qobject_cast<ThemeEditorInterface*>(oldWidget);
    PublisherInterface *pi = qobject_cast<PublisherInterface*>(oldWidget);
    if(!tei && !pi && oldWidget)
        delete oldWidget;
    QMainWindow::setCentralWidget(widget);
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
    if(m_editor && content)
    {
        m_contentAfterAnimation = content;
        editorClosed();
        return;
    }

    QString file;
    QString dir = m_installDirectory + "/sites";
    QDir path(dir + "/" + m_site->title());
    if(!content)
    {
        if(m_site->pages().count() > 0)
            content = m_site->pages().at(0);
        else if(m_site->posts().count() > 0)
            content = m_site->posts().at(0);
    }
    if(content)
    {
        file = content->url();

        QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

        m_webView = new WebView;
        WebPage *webPage = new WebPage(QWebEngineProfile::defaultProfile(), m_webView);
        m_webView->setPage(webPage);
        connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(webViewLoadFinished(bool)));
        m_webView->setUrl(QUrl("file:///" + path.absoluteFilePath(file)));
        setCursor(Qt::WaitCursor);
    }
    else
        statusBar()->showMessage("Site has no pages or posts to preview.");
}

void MainWindow::webViewLoadFinished(bool success)
{
    if(success)
    {
        setCentralWidget(m_webView);
        disconnect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(webViewLoadFinished(bool)));
    }
    else
    {
        QMessageBox::warning(this, "FlatSiteBuilder", "Unable to open webpage.");
        delete m_webView;
    }
    setCursor(Qt::ArrowCursor);
}

void MainWindow::publishSite()
{   
    QString pluginName = Plugins::actualPublishPlugin();
    PublisherInterface *pi = Plugins::getPublishPlugin(pluginName);
    if(pi)
    {
        setCentralWidget(pi);
        pi->setSitePath(m_site->deployPath());
    }
}

void MainWindow::buildSite()
{
    m_site->loadMenus();
    if(m_site->pages().count() == 0 && m_site->posts().count() == 0)
        statusBar()->showMessage("Site has no pages or posts to build.");
    else
    {
        Generator gen;
        gen.generateSite(this, m_site);
        statusBar()->showMessage(m_site->title() + " has been generated");
    }
}

void MainWindow::createSite()
{
    SiteWizard *wiz = new SiteWizard(m_installDirectory);
    connect(wiz, SIGNAL(loadSite(QString)), this, SLOT(loadProject(QString)));
    connect(wiz, SIGNAL(buildSite()), this, SLOT(buildSite()));
    wiz->show();
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

    if(!m_methodAfterAnimation.isEmpty())
    {
        QMetaObject::invokeMethod(this, m_methodAfterAnimation.toUtf8(), Qt::DirectConnection);
        m_methodAfterAnimation = "";
    }
    if(m_contentAfterAnimation)
    {
        previewSite(m_contentAfterAnimation);
        m_contentAfterAnimation = NULL;
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this && event->type() == QEvent::Resize && m_editor)
    {
        QWidget *w = centralWidget();
        if(w)
            m_editor->resize(w->size());
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

