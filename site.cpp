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

#include "site.h"
#include "generator.h"
#include "mainwindow.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QStatusBar>
#include <QTemporaryFile>
#include <QTest>

Site::Site(MainWindow *win, QString filename)
{
    QFileInfo info(filename);
    m_filename = info.fileName();
    m_win = win;
    m_sourcePath = info.path();
}

void Site::setTitle(QString title)
{
    m_title = title;
    m_deployPath = Generator::sitesPath() + "/" + m_title;
}

void Site::load()
{
    QFile file(m_sourcePath + "/Site.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        m_win->statusBar()->showMessage("Unable to open " + m_sourcePath + "/Site.xml");
        return;
    }
    QXmlStreamReader xml(&file);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "Site")
        {
            setTheme(xml.attributes().value("theme").toString());
            setTitle(xml.attributes().value("title").toString());
            setDescription(xml.attributes().value("description").toString());
            setCopyright(xml.attributes().value("copyright").toString());
            setKeywords(xml.attributes().value("keywords").toString());
            setAuthor(xml.attributes().value("author").toString());
            setPublisher(xml.attributes().value("publisher").toString());
        }
    }
    file.close();
    m_win->statusBar()->showMessage("Site has been loaded");
}

void Site::loadPages()
{
    m_pages.clear();

    QDir dir(m_sourcePath + "/pages");
    foreach(QString filename, dir.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile pagefile(m_sourcePath + "/pages/" + filename);
        if (!pagefile.open(QIODevice::ReadOnly))
        {
            m_win->statusBar()->showMessage("Unable to open " + filename);
            return;
        }

        QXmlStreamReader stream(&pagefile);
        if(stream.readNextStartElement())
        {
            if(stream.name() == "Content")
            {
                Content *content = new Content(ContentType::Page);
                content->setSource(filename);
                loadContent(content, &stream);
                addPage(content);
            }
        }
        pagefile.close();
    }
    m_win->statusBar()->showMessage("Pages have been loaded");
}

void Site::loadPosts()
{
    m_posts.clear();

    QDir posts(m_sourcePath + "/posts");
    foreach(QString filename, posts.entryList(QDir::NoDotAndDotDot | QDir::Files))
    {
        QFile postfile(m_sourcePath + "/posts/" + filename);
        if (!postfile.open(QIODevice::ReadOnly))
        {
            m_win->statusBar()->showMessage("Unable to open " + filename);
            return;
        }

        QXmlStreamReader stream(&postfile);
        if(stream.readNextStartElement())
        {
            if(stream.name() == "Content")
            {
                Content *content = new Content(ContentType::Post);
                content->setSource(filename);
                loadContent(content, &stream);
                addPost(content);
            }
        }
        postfile.close();
    }
    m_win->statusBar()->showMessage("Posts have been loaded");
}

void Site::loadContent(Content *content, QXmlStreamReader *stream)
{
    foreach(QXmlStreamAttribute att, stream->attributes())
    {
        QString attName = att.name().toString();
        QString value = att.value().toString();
        if(attName == "excerpt")
            content->setExcerpt(value);
        else if(attName == "title")
            content->setTitle(value);
        else if(attName == "menu")
            content->setMenu(value);
        else if(attName == "author")
            content->setAuthor(value);
        else if(attName == "layout")
            content->setLayout(value);
        else if(attName == "keywords")
            content->setKeywords(value);
        else if(attName == "date")
            content->setDate(QDate::fromString(value, "dd.MM.yyyy"));
        else
            content->addAttribute(attName, value);
    }
}

void Site::loadMenus()
{
    m_menus.clear();

    QFile menufile(m_sourcePath + "/Menus.xml");
    if (!menufile.open(QIODevice::ReadOnly))
    {
        m_win->statusBar()->showMessage("Unable to open " + m_sourcePath + "/Menus.xml");
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
                    addMenu(m);
                }
                else
                    menu.skipCurrentElement();
            }
        }
    }
    menufile.close();
    m_win->statusBar()->showMessage("Menus have been loaded");
}

void Site::save()
{
    QFile file(m_sourcePath + "/Site.xml");
    if(!file.open(QFile::WriteOnly))
    {
        m_win->statusBar()->showMessage("Unable to open file " + m_sourcePath + "/Site.xml");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Site");
    xml.writeAttribute("theme", theme());
    xml.writeAttribute("title", title());
    xml.writeAttribute("description", description());
    xml.writeAttribute("copyright", copyright());
    xml.writeAttribute("keywords", keywords());
    xml.writeAttribute("author", author());
    xml.writeAttribute("publisher", publisher());
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
    m_win->statusBar()->showMessage("Site has been saved");
}

void Site::saveMenus()
{
    QFile file(m_sourcePath + "/Menus.xml");
    if(!file.open(QFile::WriteOnly))
    {
        m_win->statusBar()->showMessage("Unable to open file " + m_sourcePath + "/Menus.xml");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Menus");
    foreach(Menu *menu, menus())
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
    m_win->statusBar()->showMessage("Menus have been saved");
}

QString Site::createTemporaryContent(ContentType type)
{
    QTemporaryFile file;
    if(!file.open())
    {
        m_win->statusBar()->showMessage("Unable to open file " + file.fileName());
        return "";
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Content");
    xml.writeAttribute("author", author());
    xml.writeAttribute("keywords", keywords());
    xml.writeAttribute("menu", menus().first()->name());
    xml.writeAttribute("layout", (type == ContentType::Page ? "default" : "post"));
    xml.writeAttribute("date", QString(QDate::currentDate().toString("dd.MM.yyyy")));
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

    QFileInfo info(file);
    QString filename = m_sourcePath + (type == ContentType::Page ? "/pages/" : "/posts/") + info.fileName() + ".xml";
    file.copy(filename);
    return filename;
}
