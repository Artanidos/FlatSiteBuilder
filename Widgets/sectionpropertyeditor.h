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

#ifndef SECTIONPROPERTYEDITOR_H
#define SECTIONPROPERTYEDITOR_H

#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>
#include "interfaces.h"
#include "animateableeditor.h"

class QXmlStreamReader;
class SectionPropertyEditor : public AnimateableEditor
{
    Q_OBJECT

public:
    SectionPropertyEditor();

    void setContent(QString content);
    static QString getHtml(QXmlStreamReader *xml, QString filename);

private slots:
    void closeEditor();

private:
    QLineEdit *m_cssclass;
    QLineEdit *m_style;
    QLineEdit *m_attributes;
    QLineEdit *m_id;
    QGridLayout *m_grid;
    bool m_fullwidth;
};

#endif // SECTIONPROPERTYEDITOR_H
