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
#include "animateableeditor.h"

class QXmlStreamReader;
class RowPropertyEditor : public AnimateableEditor
{
    Q_OBJECT

public:
    RowPropertyEditor();

    void setContent(QString ele);
    static QString getHtml(QXmlStreamReader *xml, QString filename);

public slots:
    void closeEditor() override;

private:
    QLineEdit *m_cssclass;
    QGridLayout *m_grid;
};

#endif // ROWPROPERTYEDITOR_H
