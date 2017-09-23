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

#include "sitesettingseditor.h"
#include "mainwindow.h"
#include "site.h"
#include "commands.h"
#include "plugins.h"
#include "generator.h"
#include <QStatusBar>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QDir>
#include <QTest>

SiteSettingsEditor::SiteSettingsEditor(MainWindow *win, Site *site)
{
    m_win = win;
    m_site = site;
    m_title = new QLineEdit();
    m_titleLabel->setText("Site Settings");
    m_filename = site->sourcePath() + "/" + site->filename();
    m_description = new QLineEdit();
    m_copyright = new QLineEdit();
    m_keywords = new QLineEdit();
    m_author = new QLineEdit();
    m_copyright->setPlaceholderText("&copy; 2017 MyCompany");
    m_publisher = new QComboBox;

    foreach(QString key, Plugins::publishPluginNames())
    {
        PublisherInterface *pi = Plugins::getPublishPlugin(key);
        m_publisher->addItem(pi->displayName(), key);
    }

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch();

    m_layout->addWidget(new QLabel("Title"), 1, 0);
    m_layout->addWidget(m_title, 2, 0);
    m_layout->addWidget(new QLabel("Description"), 3, 0);
    m_layout->addWidget(m_description, 4, 0, 1, 3);
    m_layout->addWidget(new QLabel("Copyright"), 5, 0);
    m_layout->addWidget(m_copyright, 6, 0, 1, 3);
    m_layout->addWidget(new QLabel("Keywords"), 7, 0);
    m_layout->addWidget(m_keywords, 8, 0, 1, 3);
    m_layout->addWidget(new QLabel("Author"), 9, 0);
    m_layout->addWidget(m_author, 10, 0);
    m_layout->addWidget(new QLabel("Plugin to be used for publishing"), 11, 0);
    m_layout->addWidget(m_publisher, 12, 0);
    m_layout->addLayout(vbox, 13, 0);

    load();

    connect(m_title, SIGNAL(editingFinished()), this, SLOT(titleChanged()));
    connect(m_description, SIGNAL(editingFinished()), this, SLOT(descriptionChanged()));
    connect(m_copyright, SIGNAL(editingFinished()), this, SLOT(copyrightChanged()));
    connect(m_keywords, SIGNAL(editingFinished()), this, SLOT(keywordsChanged()));
    connect(m_author, SIGNAL(editingFinished()), this, SLOT(authorChanged()));
    connect(m_publisher, SIGNAL(currentIndexChanged(QString)), this, SLOT(publisherChanged(QString)));
}

void SiteSettingsEditor::load()
{
    QString oldTitle = m_site->title();
    m_site->load();
    m_title->setText(m_site->title());
    m_description->setText(m_site->description());
    m_copyright->setText(m_site->copyright());
    m_keywords->setText(m_site->keywords());
    m_author->setText(m_site->author());
    m_publisher->setCurrentText(m_site->publisher());
    if(oldTitle != m_site->title())
    {
        QDir dir(Generator::sitesPath());
        dir.rename(oldTitle, m_site->title());
        m_win->statusBar()->showMessage("Site settings have been loaded. Site should be rebuilded. Output path has been renamed to " + m_site->title());
    }
}

void SiteSettingsEditor::save()
{
    if(m_site->title() != m_title->text())
    {
        QString oldTitle = m_site->title();
        m_site->setTitle(m_title->text());
        m_site->save();
        QDir dir(Generator::sitesPath());
        dir.rename(oldTitle, m_site->title());
        m_win->statusBar()->showMessage("Site settings have been saved. Site should be rebuilded. Output path has been renamed to " + m_title->text());
    }
    else
    {
        m_site->setAuthor(m_author->text());
        m_site->setCopyright(m_copyright->text());
        m_site->setDescription(m_description->text());
        m_site->setKeywords(m_keywords->text());
        m_site->setPublisher(m_publisher->currentText());
        m_site->save();
        m_win->statusBar()->showMessage("Site settings have been saved. Site should be rebuilded on the dashboard.");
    }
}

void SiteSettingsEditor::publisherChanged(QString publisher)
{
    if(m_site->publisher() != publisher)
        contentChanged("publisher changed");
}

void SiteSettingsEditor::titleChanged()
{
    if(m_site->title() != m_title->text())
        contentChanged("title changed");
}

void SiteSettingsEditor::authorChanged()
{
    if(m_site->author() != m_author->text())
        contentChanged("author changed");
}

void SiteSettingsEditor::descriptionChanged()
{
    if(m_site->description() != m_description->text())
        contentChanged("description changed");
}

void SiteSettingsEditor::copyrightChanged()
{
    if(m_site->copyright() != m_copyright->text())
        contentChanged("copyright changed");
}

void SiteSettingsEditor::keywordsChanged()
{
    if(m_site->keywords() != m_keywords->text())
        contentChanged("keywords changed");
}
