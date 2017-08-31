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

#include "commands.h"
#include "content.h"
#include "generator.h"
#include "site.h"
#include <QFile>
#include <QDir>
#include <QTest>

ChangeContentCommand::ChangeContentCommand(MainWindow *win, ContentEditor *ce, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_contentEditor = ce;
    m_win = win;
    fileVersionNumber += 2;
    setText(text);

    QString sitedir = m_contentEditor->site()->path().mid(m_contentEditor->site()->path().lastIndexOf("/") + 1);
    QString subdir = m_contentEditor->getContent()->contentType() == ContentType::Page ? "/pages/" : "/posts/";
    m_tempFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + m_contentEditor->getContent()->source() + "." + QString::number(fileVersionNumber);
    m_redoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + m_contentEditor->getContent()->source() + "." + QString::number(fileVersionNumber + 1);
}

ChangeContentCommand::~ChangeContentCommand()
{

}

void ChangeContentCommand::undo()
{
    QFile dest(m_contentEditor->filename());
    if(dest.exists())
        dest.remove();
    QFile::copy(m_tempFilename, m_contentEditor->filename());
    m_contentEditor->load();

    Generator gen;
    gen.generateSite(m_contentEditor->site(), m_win->editorPlugins(), m_contentEditor->getContent());
}

void ChangeContentCommand::redo()
{
    QFile redo(m_redoFilename);
    if(redo.exists())
    {
        QFile dest(m_contentEditor->filename());
        if(dest.exists())
            dest.remove();
        QFile::copy(m_redoFilename, m_contentEditor->filename());
        m_contentEditor->load();
    }
    else
    {
        QFile::copy(m_contentEditor->filename(), m_tempFilename);
        m_contentEditor->save();
        QFile::copy(m_contentEditor->filename(), m_redoFilename);
    }

    Generator gen;
    gen.generateSite(m_contentEditor->site(), m_win->editorPlugins(), m_contentEditor->getContent());
}

ChangeProjectCommand::ChangeProjectCommand(MainWindow *win, Site *site, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    fileVersionNumber += 2;
    m_site = site;
    m_win = win;
    setText(text);

    QString sitedir = m_site->path().mid(m_site->path().lastIndexOf("/") + 1);
    m_tempFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + "/Site.xml." + QString::number(fileVersionNumber);
    m_redoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + "/Site.xml." + QString::number(fileVersionNumber + 1);
}

ChangeProjectCommand::~ChangeProjectCommand()
{

}

void ChangeProjectCommand::undo()
{
    QFile dest(m_site->path() + "/Site.xml");
    if(dest.exists())
        dest.remove();
    QFile::copy(m_tempFilename, m_site->path() + "/Site.xml");
    m_win->reloadProject();
}

void ChangeProjectCommand::redo()
{
    QFile redo(m_redoFilename);
    if(redo.exists())
    {
        QFile dest(m_site->path() + "/Site.xml");
        if(dest.exists())
            dest.remove();
        QFile::copy(m_redoFilename, m_site->path() + "/Site.xml");
        m_win->reloadProject();
    }
    else
    {
        QFile::copy(m_site->path() + "/Site.xml", m_tempFilename);
        m_win->saveProject();
        QFile::copy(m_site->path() + "/Site.xml", m_redoFilename);
    }
}
