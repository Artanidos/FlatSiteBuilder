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

#include "moduldialog.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QPushButton>

ModulDialog::ModulDialog()
{
    m_result = 0;
    setWindowTitle("Insert Module");
    QGridLayout *grid = new QGridLayout();
    FlatButton *b1 = new FlatButton(":/images/text_normal.png", ":/images/text_hover.png");
    FlatButton *b2 = new FlatButton(":/images/image_normal.png", ":/images/image_hover.png");
    FlatButton *b3 = new FlatButton(":/images/slider_normal.png", ":/images/slider_hover.png");

    grid->addWidget(b1, 0, 0);
    grid->addWidget(b2, 0, 1);
    grid->addWidget(b3, 0, 2);

    QPushButton *closeButton = new QPushButton("Close");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(b1, SIGNAL(clicked()), this, SLOT(close1()));
    connect(b2, SIGNAL(clicked()), this, SLOT(close2()));
    connect(b3, SIGNAL(clicked()), this, SLOT(close3()));
}

void ModulDialog::close1()
{
    setResult(1);
    close();
}

void ModulDialog::close2()
{
    setResult(2);
    close();
}

void ModulDialog::close3()
{
    setResult(3);
    close();
}
