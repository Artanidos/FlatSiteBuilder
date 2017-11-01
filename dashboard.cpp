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
#include "flatbutton.h"
#include <QGridLayout>
#include <QLabel>
#include <QTest>
#include <QPushButton>
#include <QTextBrowser>
#include <QScrollArea>
#include <QDesktopServices>
#include <QFileDialog>

Dashboard::Dashboard(Site *site, QString defaultPath)
{
    m_site = site;
    m_defaultPath = defaultPath;

    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *layout = new QGridLayout();
    QLabel *title = new QLabel();
    title->setText("Dashboard");
    QFont fnt = title->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    title->setFont(fnt);

    m_loadButton = new FlatButton(":/images/load_normal.png", ":/images/load_hover.png", ":/images/load_pressed.png");
    m_loadButton->setToolTip("Load an existing website project");

    m_createButton = new FlatButton(":/images/create_normal.png", ":/images/create_hover.png", ":/images/create_pressed.png");
    m_createButton->setToolTip("Create a new website project");

    m_publishButton = new FlatButton(":/images/publish_normal.png", ":/images/publish_hover.png", ":/images/publish_pressed.png");
    m_publishButton->setToolTip("Upload the website to your web space provider");

    m_previewButton = new FlatButton(":/images/preview_normal.png", ":/images/preview_hover.png", ":/images/preview_pressed.png");
    m_previewButton->setToolTip("Load the website in your browser locally");

    m_buildButton = new FlatButton(":/images/build_normal.png", ":/images/build_hover.png", ":/images/build_pressed.png");
    m_buildButton->setToolTip("Build the website");

    m_info = new QLabel();
    if(m_site && !m_site->title().isEmpty())
        m_info->setText(m_site->title() + " loaded...");
    else
        m_info->setText("No site loaded yet...");

    QWidget *space = new QWidget();
    QWidget *space2 = new QWidget();
    QWidget *space3 = new QWidget();
    space->setMinimumHeight(30);
    space2->setMinimumHeight(30);
    space3->setMinimumHeight(30);
    layout->addWidget(title, 0, 0, 1, 3);
    layout->addWidget(m_info, 1, 0, 1, 3);
    layout->addWidget(space, 2, 0);
    layout->addWidget(m_loadButton, 3, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_createButton, 3, 1, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_publishButton, 3, 2, 1, 1, Qt::AlignCenter);
    layout->addWidget(space2, 4, 0);
    layout->addWidget(m_previewButton, 5, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_buildButton, 5, 1, 1, 1, Qt::AlignCenter);
    vbox->addLayout(layout);
    vbox->addStretch();
    setLayout(vbox);

    connect(m_loadButton, SIGNAL(clicked()), this, SLOT(loadClicked()));
    connect(m_createButton, SIGNAL(clicked()), this, SLOT(createClicked()));
    connect(m_publishButton, SIGNAL(clicked()), this, SLOT(publishClicked()));
    connect(m_previewButton, SIGNAL(clicked()), this, SLOT(previewClicked()));
    connect(m_buildButton, SIGNAL(clicked()), this, SLOT(buildClicked()));
}

void Dashboard::loadClicked()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter(tr("FlatSiteBuilder (*.xml);;All (*)"));
    dialog->setWindowTitle(tr("Load Site"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setDirectory(m_defaultPath);
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if (fileName.isEmpty())
        return;
    emit loadSite(fileName);
}

void Dashboard::createClicked()
{
    emit createSite();
}

void Dashboard::buildClicked()
{
    emit buildSite();
}

void Dashboard::publishClicked()
{
    emit publishSite();
}

void Dashboard::previewClicked()
{
    emit previewSite(NULL);
}

void Dashboard::siteLoaded(Site *site)
{
    m_site = site;
    if(m_site->title().isEmpty())
        m_info->setText("No site loaded yet...");
    else
        m_info->setText(m_site->title() + " loaded...");
}
