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

#ifndef ROWPROPERTYEDITOR_H
#define ROWPROPERTYEDITOR_H

#include <QLineEdit>
#include <QGridLayout>
#include "interfaces.h"

class RowPropertyEditor : public EditorInterface
{
    Q_OBJECT

public:
    RowPropertyEditor();
    ~RowPropertyEditor();

    void setContent(QDomElement ele);
    QString className() {return "RowPropertyEditor";}
    QString displayName() {return "";}
    QString tagName() {return "";}
    QImage icon() {return QImage("");}
    QString getHtml(QDomElement ele);

private slots:
    void editorClose();

private:
    QLineEdit *m_cssclass;
    QGridLayout *m_grid;
};

#endif // ROWPROPERTYEDITOR_H
