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

#ifndef CONTENTLIST_H
#define CONTENTLIST_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include "site.h"

class QUndoStack;
class FlatButton;
class ContentList  : public QWidget
{
    Q_OBJECT

public:
    ContentList(Site *site, ContentType type);
    ~ContentList();

    QTableWidget *list() {return m_list;}
    Site *site() {return m_site;}
    ContentType type() {return m_type;}
    void reload();

private slots:
    void buttonClicked();
    void tableDoubleClicked(int, int);
    void deleteContent(QObject *content);
    void editContent(QObject *content);
    void redo();
    void undo();
    void canUndoChanged(bool can);
    void canRedoChanged(bool can);
    void undoTextChanged(QString text);
    void redoTextChanged(QString text);

signals:
    void addContent();
    void editContent(QTableWidgetItem *item);

private:
    Site *m_site;
    ContentType m_type;
    QTableWidget *m_list;
    QUndoStack *m_undoStack;
    FlatButton *m_undo;
    FlatButton *m_redo;
    QString m_addedContentName;

    void addListItem(Content *content);
};

#endif // CONTENTLIST_H
