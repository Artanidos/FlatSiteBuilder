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

#ifndef SECTIONEDITOR_H
#define SECTIONEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include "roweditor.h"
#include "flatbutton.h"

class SectionEditor : public QWidget
{
    Q_OBJECT

public:
    SectionEditor(bool fullwidth = false);

    void mousePressEvent(QMouseEvent *event);
    void addRow(RowEditor *re);
    void removeRow(RowEditor *re);
    void enableColumnAcceptDrop(bool mode);
    SectionEditor *clone();
    void save(QDomDocument doc, QDomElement de);

private slots:
    void addRow();
    void close();
    void copy();
    void edit();
    void copyRowEditor(RowEditor *re);

signals:
    void sectionEditorCopied(SectionEditor*);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QVBoxLayout *m_layout;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
    bool m_fullwidth;

    ContentEditor* getContentEditor();
};

#endif // SECTIONEDITOR_H
