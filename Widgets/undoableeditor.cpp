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

#include "undoableeditor.h"
#include <QUndoCommand>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFileInfo>
#include <QDir>

UndoableEditor::UndoableEditor(QString title, QString filename)
{
    m_filename = filename;
    m_undoStack = new QUndoStack;
    m_undo = new FlatButton(":/images/undo_normal.png", ":/images/undo_hover.png", "", ":/images/undo_disabled.png");
    m_redo = new FlatButton(":/images/redo_normal.png", ":/images/redo_hover.png", "", ":/images/redo_disabled.png");
    m_undo->setToolTip("Undo");
    m_redo->setToolTip("Redo");
    m_undo->setEnabled(false);
    m_redo->setEnabled(false);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch(0);
    hbox->addWidget(m_undo);
    hbox->addWidget(m_redo);

    QLabel *titleLabel = new QLabel(title);
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    m_layout = new QGridLayout;
    m_layout->addWidget(titleLabel, 0, 0);
    m_layout->addLayout(hbox, 0, 2);
    setLayout(m_layout);

    connect(m_redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(m_undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SLOT(canUndoChanged(bool)));
    connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SLOT(canRedoChanged(bool)));
    connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
    connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SLOT(redoTextChanged(QString)));
}

UndoableEditor::~UndoableEditor()
{
    disconnect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SLOT(canUndoChanged(bool)));
    disconnect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SLOT(canRedoChanged(bool)));
    disconnect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
    disconnect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SLOT(redoTextChanged(QString)));
    delete m_undoStack;
}

void UndoableEditor::contentChanged(QString text)
{
    QUndoCommand *changeCommand = new ChangeFileCommand(this, m_filename, text);
    m_undoStack->push(changeCommand);
}

int fileVersionNumber = 0;

ChangeFileCommand::ChangeFileCommand(UndoableEditor *editor, QString filename, QString text, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    fileVersionNumber++;
    m_editor = editor;
    m_filename = filename;
    setText(text);

    QFileInfo info(filename);
    QString dir = info.dir().dirName();
    QString file = info.fileName();
    m_undoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + dir + "/" + file + "." + QString::number(fileVersionNumber)  + ".undo";
    m_redoFilename = QDir::tempPath() + "/FlatSiteBuilder/" + dir + "/" + file + "." + QString::number(fileVersionNumber)  + ".redo";
}

void ChangeFileCommand::undo()
{
    QFile dest(m_filename);
    if(dest.exists())
        dest.remove();
    QFile::copy(m_undoFilename, m_filename);
    m_editor->load();
}

void ChangeFileCommand::redo()
{
    QFile redo(m_redoFilename);
    if(redo.exists())
    {
        QFile dest(m_filename);
        if(dest.exists())
            dest.remove();
        QFile::copy(m_redoFilename, m_filename);
        m_editor->load();
    }
    else
    {
        QFile::copy(m_filename, m_undoFilename);
        m_editor->save();
        QFile::copy(m_filename, m_redoFilename);
    }
}
