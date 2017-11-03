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
#include "menulist.h"
#include "site.h"
#include "contenteditor.h"
#include "contentlist.h"
#include "undoableeditor.h"
#include <QFile>
#include <QDir>
#include <QTest>

ChangeContentCommand::ChangeContentCommand(QMainWindow *win, ContentEditor *ce, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_win = win;
    m_contentEditor = ce;
    fileVersionNumber++;
    setText(text);

    QString sitedir = m_contentEditor->site()->sourcePath().mid(m_contentEditor->site()->sourcePath().lastIndexOf("/") + 1);
    QString subdir = m_contentEditor->getContent()->contentType() == ContentType::Page ? "/pages/" : "/posts/";
    m_tempFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + m_contentEditor->getContent()->source() + "." + QString::number(fileVersionNumber) + ".undo";
    m_redoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + m_contentEditor->getContent()->source() + "." + QString::number(fileVersionNumber) + ".redo";
}

void ChangeContentCommand::undo()
{
    QFile dest(m_contentEditor->filename());
    if(dest.exists())
        dest.remove();
    QFile::copy(m_tempFilename, m_contentEditor->filename());
    m_contentEditor->load();

    Generator gen;
    gen.generateSite(m_win, m_contentEditor->site(), m_contentEditor->getContent());
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
    gen.generateSite(m_win, m_contentEditor->site(), m_contentEditor->getContent());
}

DeleteContentCommand::DeleteContentCommand(ContentList *cl, QString filename, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_contentList = cl;
    m_filename = filename;
    fileVersionNumber++;
    setText(text);
    QFileInfo info(filename);
    QString sitedir = m_contentList->site()->sourcePath().mid(m_contentList->site()->sourcePath().lastIndexOf("/") + 1);
    QString subdir = m_contentList->type() == ContentType::Page ? "/pages/" : "/posts/";
    m_undoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + sitedir + subdir + info.fileName() + "." + QString::number(fileVersionNumber) + ".undo";
}

void DeleteContentCommand::undo()
{
    QFile file(m_undoFilename);
    file.copy(m_filename);
    m_contentList->reload();
}

void DeleteContentCommand::redo()
{
    QFile file(m_filename);
    QFile undo(m_undoFilename);
    if(!undo.exists())
        file.copy(m_undoFilename);
    file.remove();
    m_contentList->reload();
}

RenameContentCommand::RenameContentCommand(ContentEditor *ce, QString oldname, QString newname, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    m_contentEditor = ce;
    m_oldname = oldname;
    m_newname = newname;
    setText(text);
}

void RenameContentCommand::undo()
{
    QFile file(m_newname);
    file.copy(m_oldname);
    file.remove();
    m_contentEditor->contentRenamed(m_oldname);
}

void RenameContentCommand::redo()
{
    QFile newFile(m_newname);
    if(newFile.exists())
    {
        QFileInfo info(m_newname);
        int start = info.baseName().indexOf("(");
        int end = info.baseName().indexOf(")");
        int number = 0;
        QString base = info.baseName();
        if(start > 0 && end > start)
        {
            number = QString(base.mid(start + 1, base.length() - end)).toInt();
            base = base.mid(0, start);
        }
        do
        {
            m_newname = info.path() + "/" + base + "(" + QString::number(++number) + ")." + info.suffix();
            newFile.setFileName(m_newname);
        }
        while(newFile.exists());
    }
    QFile file(m_oldname);
    file.copy(m_newname);
    file.remove();
    m_contentEditor->contentRenamed(m_newname);
}
