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
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QHeaderView>

SliderEditor::SliderEditor()
    : AbstractEditor()
{
    m_list = new QTableWidget(0, 2, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name";
    m_list->setHorizontalHeaderLabels(labels);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);
    QPushButton *addSlide = new QPushButton("Add Slide");
    addSlide->setMaximumWidth(120);
    m_deleteButton = new QPushButton();
    m_deleteButton->setText("Delete");
    m_deleteButton->setMaximumWidth(120);
    m_deleteButton->setEnabled(false);
    m_deleteButton->setToolTip("Delete all marked items");
    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("Slider Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(cancel);
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(addSlide, 1, 0);
    grid->addWidget(m_list, 2, 0, 1, 3);
    grid->addWidget(m_deleteButton, 3, 0);
    grid->addLayout(hbox, 4, 0, 1, 3);
    setLayout(grid);

    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
}

void SliderEditor::setContent(QDomElement element)
{
    m_element = element;
}

void SliderEditor::save()
{
    if(m_changed)
    {
        if(m_element.isNull())
        {
            QDomDocument doc;
            m_element = doc.createElement("Slider");
        }
        //m_element.setAttribute("src", m_temp);
        //m_element.setAttribute("animation", m_animationCombo->currentData(Qt::UserRole).toString());
    }
    emit close(this);
}

void SliderEditor::cancel()
{
    m_changed = false;
    emit close(this);
}
