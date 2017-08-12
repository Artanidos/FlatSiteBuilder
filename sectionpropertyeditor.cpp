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

#include "sectionpropertyeditor.h"
#include <QPushButton>
#include <QLabel>

SectionPropertyEditor::SectionPropertyEditor()
    : AbstractEditor()
{
    m_grid = new QGridLayout();
    m_grid->setMargin(0);

    m_cssclass = new QLineEdit();
    m_style = new QLineEdit();
    m_attributes = new QLineEdit();

    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("Section Module Settings");
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
    m_grid->addWidget(new QLabel("Style"), 3, 0);
    m_grid->addWidget(m_style, 4, 0);
    m_grid->addWidget(new QLabel("Aditional Attributes"), 5, 0);
    m_grid->addWidget(m_attributes, 6, 0);
    m_grid->addLayout(vbox, 7, 0);
    m_grid->addLayout(hbox, 8, 0);
    setLayout(m_grid);

    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(m_cssclass, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_style, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_attributes, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
}

SectionPropertyEditor::~SectionPropertyEditor()
{
    delete m_grid;
}

void SectionPropertyEditor::save()
{
    if(m_changed)
    {
        if(m_element.isNull())
        {
            m_element = m_doc.createElement("Section");
        }
        m_element.setAttribute("cssclass", m_cssclass->text());
        m_element.setAttribute("style", m_style->text());
        m_element.setAttribute("attributes", m_attributes->text());
    }
    emit close(this);
}

void SectionPropertyEditor::cancel()
{
    m_changed = false;
    emit close(this);
}

void SectionPropertyEditor::setContent(QDomElement element)
{
    m_element = element;
    m_cssclass->setText(m_element.attribute("cssclass", ""));
    m_style->setText(m_element.attribute("style", ""));
    m_attributes->setText(m_element.attribute("attributes", ""));
    m_changed = false;
}
