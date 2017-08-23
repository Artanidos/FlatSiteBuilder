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
#include "flatbutton.h"
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
    m_id = new QLineEdit;

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    QLabel *titleLabel = new QLabel("Section Module Settings");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();

    m_grid->addWidget(titleLabel, 0, 0);
    m_grid->addWidget(close, 0, 1, 1, 1, Qt::AlignRight);
    m_grid->addWidget(new QLabel("CSS Class"), 1, 0);
    m_grid->addWidget(m_cssclass, 2, 0, 1, 2);
    m_grid->addWidget(new QLabel("Style"), 3, 0);
    m_grid->addWidget(m_style, 4, 0, 1, 2);
    m_grid->addWidget(new QLabel("Aditional Attributes"), 5, 0);
    m_grid->addWidget(m_attributes, 6, 0, 1, 2);
    m_grid->addWidget(new QLabel("Id"), 7, 0);
    m_grid->addWidget(m_id, 8, 0, 1, 2);
    m_grid->addLayout(vbox, 9, 0);
    setLayout(m_grid);

    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_cssclass, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_style, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_attributes, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_id, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));

}

SectionPropertyEditor::~SectionPropertyEditor()
{
    delete m_grid;
}

void SectionPropertyEditor::closeEditor()
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
        m_element.setAttribute("id", m_id->text());
    }
    emit close();
}

void SectionPropertyEditor::setContent(QDomElement element)
{
    m_element = element;
    m_cssclass->setText(m_element.attribute("cssclass", ""));
    m_style->setText(m_element.attribute("style", ""));
    m_attributes->setText(m_element.attribute("attributes", ""));
    m_id->setText(m_element.attribute("id", ""));
    m_changed = false;
}
