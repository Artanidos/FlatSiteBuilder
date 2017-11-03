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

#ifndef UNDOABLEEDITOR_H
#define UNDOABLEEDITOR_H

#include <QWidget>
#include "flatbutton.h"
#include <QUndoStack>

extern int fileVersionNumber;

class QGridLayout;
class QMainWindow;
class UndoableEditor : public QWidget
{
    Q_OBJECT

public:
    UndoableEditor();
    ~UndoableEditor();

    virtual void load() = 0;
    virtual void save() = 0;

    void setWindow(QMainWindow *win) {m_win = win;}

protected:
    void contentChanged(QString text);

private slots:
    void canUndoChanged(bool can) {m_undo->setEnabled(can);}
    void canRedoChanged(bool can) {m_redo->setEnabled(can);}
    void undoTextChanged(QString text) {m_undo->setToolTip("Undo " + text);}
    void redoTextChanged(QString text) {m_redo->setToolTip("Redo " + text);}
    void undo() {m_undoStack->undo();}
    void redo() {m_undoStack->redo();}

protected:
    QGridLayout *m_layout;
    QUndoStack *m_undoStack;
    QString m_filename;
    QMainWindow *m_win;
    QLabel *m_titleLabel;

private:
    FlatButton *m_undo;
    FlatButton *m_redo;
};

class ChangeFileCommand : public QUndoCommand
{
public:
    ChangeFileCommand(UndoableEditor *editor, QString filename, QString text, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    QString m_undoFilename;
    QString m_redoFilename;
    QString m_filename;
    UndoableEditor *m_editor;
};

#endif // UNDOABLEEDITOR_H
