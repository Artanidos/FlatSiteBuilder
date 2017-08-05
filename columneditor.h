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

#ifndef COLUMNEDITOR_H
#define COLUMNEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QDomElement>

class ElementEditor;
class ContentEditor;
class ColumnEditor : public QWidget
{
    Q_OBJECT

public:
    ColumnEditor();

    ColumnEditor* clone();
    void addElement(ElementEditor *);
    void save(QDomDocument doc, QDomElement de);
    void setSpan(int s) {m_span = s;}
    int span() {return m_span;}

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

public slots:
    void addElement();
    void copyElement(ElementEditor*);

private:
    QVBoxLayout *m_layout;
    int m_span;

    ContentEditor* getContentEditor();
};

#endif // COLUMNEDITOR_H
