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

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

extern int fileVersionNumber;

class ContentList;
class ContentEditor;
class MainWindow;
class Site;
class ChangeContentCommand : public QUndoCommand
{
public:
    ChangeContentCommand(MainWindow *win, ContentEditor *ce, QString text, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    ContentEditor *m_contentEditor;
    QString m_tempFilename;
    QString m_redoFilename;
    MainWindow *m_win;
};

class DeleteContentCommand : public QUndoCommand
{
public:
    DeleteContentCommand(ContentList *cl, QString filename, QString text, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    ContentList *m_contentList;
    QString m_filename;
    QString m_undoFilename;
};

class RenameContentCommand : public QUndoCommand
{
public:
    RenameContentCommand(ContentEditor *ce, QString oldname, QString newname, QString text, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    ContentEditor *m_contentEditor;
    QString m_oldname;
    QString m_newname;
};

#endif // COMMANDS_H
