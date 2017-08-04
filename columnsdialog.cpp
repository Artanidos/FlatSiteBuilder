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

#include "columnsdialog.h"
#include "flatbutton.h"

#include <QPushButton>
#include <QHBoxLayout>

ColumnsDialog::ColumnsDialog()
{
    m_result = 0;
    setWindowTitle("Insert Columns");
    QGridLayout *grid = new QGridLayout();
    FlatButton *b1 = new FlatButton(":/images/columns1.png", ":/images/columns1_hover.png");
    FlatButton *b2 = new FlatButton(":/images/columns2.png", ":/images/columns2_hover.png");
    FlatButton *b3 = new FlatButton(":/images/columns3.png", ":/images/columns3_hover.png");
    FlatButton *b4 = new FlatButton(":/images/columns4.png", ":/images/columns4_hover.png");
    FlatButton *b5 = new FlatButton(":/images/columns5.png", ":/images/columns5_hover.png");
    FlatButton *b6 = new FlatButton(":/images/columns6.png", ":/images/columns6_hover.png");
    FlatButton *b7 = new FlatButton(":/images/columns7.png", ":/images/columns7_hover.png");
    FlatButton *b8 = new FlatButton(":/images/columns8.png", ":/images/columns8_hover.png");
    FlatButton *b9 = new FlatButton(":/images/columns9.png", ":/images/columns9_hover.png");
    FlatButton *b10 = new FlatButton(":/images/columns10.png", ":/images/columns10_hover.png");
    FlatButton *b11 = new FlatButton(":/images/columns11.png", ":/images/columns11_hover.png");
    grid->addWidget(b1, 0, 0);
    grid->addWidget(b2, 0, 1);
    grid->addWidget(b3, 0, 2);
    grid->addWidget(b4, 1, 0);
    grid->addWidget(b5, 1, 1);
    grid->addWidget(b6, 1, 2);
    grid->addWidget(b7, 3, 0);
    grid->addWidget(b8, 3, 1);
    grid->addWidget(b9, 3, 2);
    grid->addWidget(b10, 4, 0);
    grid->addWidget(b11, 4, 1);

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
    connect(b4, SIGNAL(clicked()), this, SLOT(close4()));
    connect(b5, SIGNAL(clicked()), this, SLOT(close5()));
    connect(b6, SIGNAL(clicked()), this, SLOT(close6()));
    connect(b7, SIGNAL(clicked()), this, SLOT(close7()));
    connect(b8, SIGNAL(clicked()), this, SLOT(close8()));
    connect(b9, SIGNAL(clicked()), this, SLOT(close9()));
    connect(b10, SIGNAL(clicked()), this, SLOT(close10()));
    connect(b11, SIGNAL(clicked()), this, SLOT(close11()));
}

void ColumnsDialog::close1()
{
    setResult(1);
    close();
}

void ColumnsDialog::close2()
{
    setResult(2);
    close();
}

void ColumnsDialog::close3()
{
    setResult(3);
    close();
}

void ColumnsDialog::close4()
{
    setResult(4);
    close();
}

void ColumnsDialog::close5()
{
    setResult(5);
    close();
}

void ColumnsDialog::close6()
{
    setResult(6);
    close();
}

void ColumnsDialog::close7()
{
    setResult(7);
    close();
}

void ColumnsDialog::close8()
{
    setResult(8);
    close();
}

void ColumnsDialog::close9()
{
    setResult(9);
    close();
}

void ColumnsDialog::close10()
{
    setResult(10);
    close();
}

void ColumnsDialog::close11()
{
    setResult(11);
    close();
}
