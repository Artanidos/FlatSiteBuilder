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

#ifndef ROWEDITOR_H
#define ROWEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QDomElement>
#include "elementeditor.h"
#include "flatbutton.h"

class RowEditor : public QWidget
{
    Q_OBJECT

public:
    RowEditor(bool clone = false);

    void mousePressEvent(QMouseEvent *event);
    void enableColumnAcceptDrop(bool mode);
    void addColumn(ColumnEditor *, int column);
    RowEditor* clone();
    void save(QDomDocument doc, QDomElement de);
    void setContent(QDomElement row);
    QDomElement content();
    void setCssClass(QString cls) {m_cssclass = cls;}

signals:
    void beginDrag();
    void rowEditorCopied(RowEditor*);

private slots:
    void close();
    void copy();
    void edit();
    void addColumns();

private:
    QGridLayout *m_layout;
    QRect m_highlightedRect;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
    Hyperlink *m_addColumns;
    QString m_cssclass;
    QDomDocument m_doc;

    ContentEditor* getContentEditor();
};

#endif // ROWEDITOR_H
