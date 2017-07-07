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

#include "dashboard.h"

#include <QGridLayout>
#include <QLabel>

Dashboard::Dashboard()
{
    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *layout = new QGridLayout();
    QLabel *title = new QLabel();
    title->setText("Dashboard");
    QFont fnt = title->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    title->setFont(fnt);
    layout->addWidget(title, 0, 0);
    vbox->addLayout(layout);
    vbox->addStretch();
    setLayout(vbox);
}
