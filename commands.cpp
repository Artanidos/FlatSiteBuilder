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
#include <QFile>
#include <QDir>
#include <QTest>

ChangeContentCommand::ChangeContentCommand(ContentEditor *ce, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_contentEditor = ce;
    fileVersionNumber += 2;
    setText(text);
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
}

void ChangeContentCommand::redo()
{
    QString sitedir = m_contentEditor->site()->path().mid(m_contentEditor->site()->path().lastIndexOf("/") + 1);
    QString subdir = m_contentEditor->content()->contentType() == ContentType::Page ? "/pages/" : "/posts/";
    m_tempFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + m_contentEditor->content()->source() + "." + QString::number(fileVersionNumber);
    m_redoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + m_contentEditor->content()->source() + "." + QString::number(fileVersionNumber + 1);

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
}
