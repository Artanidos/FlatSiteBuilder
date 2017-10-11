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

#include "sitewizard.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QXmlStreamWriter>
#include <QTest>

SiteWizard::SiteWizard()
{
    addPage(new IntroPage);
    addPage(new SiteInfoPage);
    addPage(new ConclusionPage);

    setWindowTitle(tr("Site Wizard"));
}

void SiteWizard::accept()
{
    QString siteName = field("siteName").toString();
    QString description = field("description").toString();
    QString copyright = field("copyright").toString();
    QString path = QDir::homePath() + "/FlatSiteBuilder/sources/" + siteName.toLower();

    QDir dir(QDir::homePath() + "/FlatSiteBuilder/sources/");
    dir.mkdir(siteName.toLower());
    dir.cd(siteName.toLower());
    dir.mkdir("pages");
    dir.mkdir("posts");
    dir.mkdir("includes");
    dir.mkdir("layouts");
    dir.mkdir("assets");
    dir.cd("assets");
    dir.mkdir("css");
    dir.mkdir("fonts");
    dir.mkdir("js");
    dir.mkdir("images");

    QFile file(path + "/Site.xml");
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/Site.xml";
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.writeStartDocument();
    xml.writeStartElement("Site");
    xml.writeAttribute("theme", field("theme").toString());
    xml.writeAttribute("title", siteName);
    if(!description.isEmpty())
        xml.writeAttribute("description", description);
    if(!copyright.isEmpty())
        xml.writeAttribute("copyright", copyright);
    xml.writeEndElement(); // Site
    xml.writeEndDocument();
    file.close();

    QFile menu(path + "/Menus.xml");
    if(!menu.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/Menus.xml";
        return;
    }
    QXmlStreamWriter men(&menu);
    men.writeStartDocument();
    men.writeStartElement("Menus");
    men.writeStartElement("Menu");
    men.writeAttribute("name", "default");
    men.writeStartElement("Item");
    men.writeAttribute("title", "Index");
    men.writeAttribute("url", "index.html");
    men.writeEndElement(); // Item
    men.writeEndElement(); // Menu
    men.writeEndElement(); // Menus
    men.writeEndDocument();
    menu.close();

    QFile index(path + "/pages/index.xml");
    if(!index.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/pages/index.xml";
        return;
    }
    QXmlStreamWriter ind(&index);
    ind.writeStartDocument();
    ind.writeStartElement("Content");
    ind.writeAttribute("title", "Index");
    ind.writeAttribute("menu", "default");
    ind.writeAttribute("author", "admin");
    ind.writeAttribute("layout", "default");
    ind.writeAttribute("date", QString(QDate::currentDate().toString("dd.MM.yyyy")));
    ind.writeStartElement("Section");
    ind.writeStartElement("Row");
    ind.writeStartElement("Column");
    ind.writeAttribute("span", "12");
    ind.writeStartElement("Text");
    ind.writeCDATA("<h1>Welcome</h1>");
    ind.writeEndElement(); // Content
    ind.writeEndElement(); // Section
    ind.writeEndElement(); // Row
    ind.writeEndElement(); // Column
    ind.writeEndElement(); // Text
    ind.writeEndDocument();
    index.close();

    QDialog::accept();

    emit loadSite(path + "/Site.xml");
    emit buildSite();
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel(tr("This wizard will generate a skeleton website. "
                          "You simply need to specify the site name and set a "
                          "few options to produce the site."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

SiteInfoPage::SiteInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Site Information"));
    setSubTitle(tr("Specify basic information about the site for which you "
                   "want to generate site files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/icon64.png"));

    m_siteNameLabel = new QLabel("&Site title:");
    m_siteNameLineEdit = new QLineEdit;
    m_siteNameLabel->setBuddy(m_siteNameLineEdit);
    m_siteNameLineEdit->setPlaceholderText("Site title");

    m_descriptionLabel = new QLabel("&Description:");
    m_descriptionLineEdit = new QLineEdit;
    m_descriptionLabel->setBuddy(m_descriptionLineEdit);
    m_descriptionLineEdit->setPlaceholderText("Site description");

    m_copyrightLabel = new QLabel("&Copyright");
    m_copyrightLineEdit = new QLineEdit;
    m_copyrightLabel->setBuddy(m_copyrightLineEdit);
    m_copyrightLineEdit->setPlaceholderText("&copy; 2017 My Company. All Rights Reserved.");

    m_themeLabel = new QLabel("&Theme");
    m_theme = new QComboBox;
    m_themeLabel->setBuddy(m_theme);
    m_theme->addItem("default");
    m_theme->addItem("himu");

    registerField("siteName*", m_siteNameLineEdit);
    registerField("description", m_descriptionLineEdit);
    registerField("copyright", m_copyrightLineEdit);
    registerField("theme", m_theme, "currentText");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_siteNameLabel, 0, 0);
    layout->addWidget(m_siteNameLineEdit, 0, 1);
    layout->addWidget(m_descriptionLabel, 1, 0);
    layout->addWidget(m_descriptionLineEdit, 1, 1);
    layout->addWidget(m_copyrightLabel, 2, 0);
    layout->addWidget(m_copyrightLineEdit, 2, 1);
    layout->addWidget(m_themeLabel, 3, 0);
    layout->addWidget(m_theme, 3, 1);
    setLayout(layout);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the site skeleton.").arg(finishText));
}
