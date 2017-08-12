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

#include "rowpropertyeditor.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

RowPropertyEditor::RowPropertyEditor()
    : AbstractEditor()
{
    m_grid = new QGridLayout();
    m_grid->setMargin(0);

    m_cssclass = new QLineEdit();

    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("Row Module Settings");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();
    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(cancel);
    m_grid->addWidget(titleLabel, 0, 0);
    m_grid->addWidget(new QLabel("CSS Class"), 1, 0);
    m_grid->addWidget(m_cssclass, 2, 0);
    m_grid->addLayout(vbox, 3, 0);
    m_grid->addLayout(hbox, 4, 0);
    setLayout(m_grid);

    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(m_cssclass, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
}

RowPropertyEditor::~RowPropertyEditor()
{
    delete m_grid;
}

void RowPropertyEditor::save()
{
    if(m_changed)
    {
        if(m_element.isNull())
        {
            m_element = m_doc.createElement("Row");
        }
        m_element.setAttribute("cssclass", m_cssclass->text());
    }
    emit close(this);
}

void RowPropertyEditor::cancel()
{
    m_changed = false;
    emit close(this);
}

void RowPropertyEditor::setContent(QDomElement element)
{
    m_element = element;
    m_cssclass->setText(m_element.attribute("cssclass", ""));
    m_changed = false;
}
