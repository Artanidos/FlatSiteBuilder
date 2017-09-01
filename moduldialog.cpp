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
#include "interfaces.h"
#include <QGridLayout>
#include <QPushButton>
#include <QMap>
#include <QLabel>

ModulDialog::ModulDialog()
{
    m_result = "";
    setWindowTitle("Insert Module");
    m_grid = new QGridLayout();
    FlatButton *textButton = createButton(QImage(":/images/text.png"), "Text");
    m_grid->addWidget(textButton, 0, 0);

    QPushButton *cancelButton = new QPushButton("Cancel");
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(m_grid);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(textButton, SIGNAL(clicked()), this, SLOT(close1()));
}

FlatButton* ModulDialog::createButton(QImage icon, QString text)
{
    FlatButton *btn = new FlatButton;
    QPixmap pmNormal = QPixmap::fromImage(QImage(":/images/module_normal.png"));
    QPixmap pmHover = QPixmap::fromImage(QImage(":/images/module_hover.png"));
    QLabel title;
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor("#999999"));
    pal.setColor(QPalette::Foreground, QColor("#000000"));
    title.setPalette(pal);
    title.setText(text);
    title.setFixedWidth(90);
    title.render(&pmNormal, QPoint(80, 40));
    title.render(&pmHover, QPoint(80, 40));

    QLabel iconLabel;
    iconLabel.setPixmap(QPixmap::fromImage(icon));
    iconLabel.render(&pmNormal, QPoint(33, 33));
    iconLabel.render(&pmHover, QPoint(33, 33));

    btn->setNormalPixmap(pmNormal);
    btn->setHoverPixmap(pmHover);
    return btn;
}

void ModulDialog::registerPlugins(QMap<QString, EditorInterface*> plugins)
{
    int row = 0;
    int col = 1;

    foreach(QString name, plugins.keys())
    {
        if(name != "RowPropertyEditor" && name != "SectionPropertyEditor" && name != "TextEditor")
        {
            EditorInterface *plugin = plugins[name];
            FlatButton *btn = createButton(plugin->icon(), plugin->displayName());
            btn->setReturnCode(name);
            m_grid->addWidget(btn, row, col++);
            connect(btn, SIGNAL(clicked(QString)), this, SLOT(close2(QString)));
            if(col == 2)
            {
                row++;
                col = 0;
            }
        }
    }
}

void ModulDialog::close1()
{
    setResult("TextEditor");
    close();
}

void ModulDialog::close2(QString rc)
{
    setResult(rc);
    close();
}
