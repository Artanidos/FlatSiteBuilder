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

#ifndef PAGEEDITOR_H
#define PAGEEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "sectioneditor.h"

class PageEditor : public QWidget
{
    Q_OBJECT

public:
    PageEditor();

    void addSection(SectionEditor *se);
    void removeSection(SectionEditor *se);
    void enableColumnAcceptDrop(bool mode);
    void enableSectionAcceptDrop(bool mode);
    QList<SectionEditor*> sections();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void addSection();
    void addFullSection();
    void copySection(SectionEditor *);

private:
    QVBoxLayout *m_layout;

    ContentEditor* getContentEditor();
};

#endif // PAGEEDITOR_H
