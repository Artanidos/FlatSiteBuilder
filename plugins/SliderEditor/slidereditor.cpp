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

#include "slidereditor.h"
#include "flatbutton.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include <QHBoxLayout>

SliderEditor::SliderEditor()
{
    m_changed = false;
    setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    m_adminlabel = new QLineEdit;
    m_adminlabel->setMaximumWidth(200);
    QLabel *titleLabel = new QLabel("Slider Module Plugin");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    QPushButton *addSlide = new QPushButton("Add Slide");
    addSlide->setMaximumWidth(120);

    m_list = new QTableWidget(0, 2, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name";
    m_list->setHorizontalHeaderLabels(labels);

    m_deleteButton = new QPushButton();
    m_deleteButton->setText("Delete");
    m_deleteButton->setMaximumWidth(120);
    m_deleteButton->setEnabled(false);
    m_deleteButton->setToolTip("Delete all marked items");

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2, 1, 1, Qt::AlignRight);
    grid->addWidget(addSlide, 1, 0);
    grid->addWidget(m_list, 2, 0, 1, 3);
    grid->addWidget(m_deleteButton, 3, 0);
    grid->addWidget(new QLabel("Admin Label"), 4, 0);
    grid->addWidget(m_adminlabel, 5, 0);

    setLayout(grid);

    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
}

void SliderEditor::setContent(QDomElement element)
{
    m_element = element;
    m_adminlabel->setText(m_element.attribute("adminlabel"));
}

void SliderEditor::closeEditor()
{
    if(m_changed)
    {
        if(m_element.isNull())
        {
            m_element = m_doc.createElement("Sample");
        }
        m_element.setAttribute("adminlabel", m_adminlabel->text());
    }
    emit close();
}

QString SliderEditor::getHtml(QDomElement)
{
    //QString sampleproperty = ele.attribute("sampleproperty", "");
    return "<div></div>";
}
