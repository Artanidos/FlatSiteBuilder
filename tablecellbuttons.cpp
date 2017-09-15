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

#include "tablecellbuttons.h"
#include "flatbutton.h"
#include <QHBoxLayout>

TableCellButtons::TableCellButtons()
{
    m_delete = new FlatButton(":/images/trash_normal.png", ":/images/trash_hover.png");
    m_edit = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_edit->setToolTip("Edit Item");
    m_delete->setToolTip("Delete Item");

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(m_edit);
    hbox->addWidget(m_delete);
    setLayout(hbox);

    connect(m_delete, SIGNAL(clicked()), this, SLOT(deleteItemClicked()));
    connect(m_edit, SIGNAL(clicked()), this, SLOT(editItemClicked()));
}

void TableCellButtons::deleteItemClicked()
{
    emit deleteItem(m_item);
}

void TableCellButtons::editItemClicked()
{
    emit editItem(m_item);
}
