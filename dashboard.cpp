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
#include <QTest>
#include <QPushButton>
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
    m_loadButton = new QPushButton();
    m_loadButton->setIcon(QIcon(":/images/load_normal.png"));
    m_loadButton->setIconSize(QSize(100, 100));
    m_loadButton->setFlat(true);
    m_loadButton->installEventFilter(this);

    m_createButton = new QPushButton();
    m_createButton->setIcon(QIcon(":/images/create_normal.png"));
    m_createButton->setIconSize(QSize(100, 100));
    m_createButton->setFlat(true);
    m_createButton->installEventFilter(this);

    m_publishButton = new QPushButton();
    m_publishButton->setIcon(QIcon(":/images/publish_normal.png"));
    m_publishButton->setIconSize(QSize(100, 100));
    m_publishButton->setFlat(true);
    m_publishButton->installEventFilter(this);

    m_previewButton = new QPushButton();
    m_previewButton->setIcon(QIcon(":/images/preview_normal.png"));
    m_previewButton->setIconSize(QSize(100, 100));
    m_previewButton->setFlat(true);
    m_previewButton->installEventFilter(this);

    m_info = new QLabel();
    if(m_site)
        m_info->setText(m_site->title() + " loaded...");
    else
        m_info->setText("No site loaded yet...");
    QWidget *space = new QWidget();
    QWidget *space2 = new QWidget();
    space->setMinimumHeight(30);
    space2->setMinimumHeight(30);
    layout->addWidget(title, 0, 0, 1, 3);
    layout->addWidget(m_info, 1, 0, 1, 3);
    layout->addWidget(space, 2, 0);
    layout->addWidget(m_loadButton, 3, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_createButton, 3, 1, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_publishButton, 3, 2, 1, 1, Qt::AlignCenter);
    layout->addWidget(space2, 4, 0);
    layout->addWidget(m_previewButton, 5, 0, 1, 1, Qt::AlignCenter);
    vbox->addLayout(layout);
    vbox->addStretch();
    setLayout(vbox);

    connect(m_loadButton, SIGNAL(clicked(bool)), this, SLOT(loadClicked()));
    connect(m_createButton, SIGNAL(clicked(bool)), this, SLOT(createClicked()));
    connect(m_publishButton, SIGNAL(clicked(bool)), this, SLOT(publishClicked()));
    connect(m_previewButton, SIGNAL(clicked(bool)), this, SLOT(previewClicked()));
}

bool Dashboard::eventFilter(QObject * watched, QEvent * event)
{
    QPushButton * button = qobject_cast<QPushButton*>(watched);
    if (!button)
        return false;

    if (event->type() == QEvent::Enter)
    {
        if(button == m_loadButton)
            button->setIcon(QIcon(":/images/load_hover.png"));
        else if(button == m_createButton)
            button->setIcon(QIcon(":/images/create_hover.png"));
        else if(button == m_publishButton)
            button->setIcon(QIcon(":/images/publish_hover.png"));
        else if(button == m_previewButton)
            button->setIcon(QIcon(":/images/preview_hover.png"));
        return true;
    }

    else if (event->type() == QEvent::Leave)
    {
        if(button == m_loadButton)
            button->setIcon(QIcon(":/images/load_normal.png"));
        else if(button == m_createButton)
            button->setIcon(QIcon(":/images/create_normal.png"));
        else if(button == m_publishButton)
            button->setIcon(QIcon(":/images/publish_normal.png"));
        else if(button == m_previewButton)
            button->setIcon(QIcon(":/images/preview_normal.png"));
        return true;
    }

    else if(event->type() == QEvent::MouseButtonPress)
    {
        if(button == m_loadButton)
            button->setIcon(QIcon(":/images/load_pressed.png"));
        else if(button == m_createButton)
            button->setIcon(QIcon(":/images/create_pressed.png"));
        else if(button == m_publishButton)
            button->setIcon(QIcon(":/images/publish_pressed.png"));
        else if(button == m_previewButton)
            button->setIcon(QIcon(":/images/preview_pressed.png"));
    }

    else if(event->type() == QEvent::MouseButtonRelease)
    {
        if(button == m_loadButton)
            button->setIcon(QIcon(":/images/load_hover.png"));
        else if(button == m_createButton)
            button->setIcon(QIcon(":/images/create_hover.png"));
        else if(button == m_publishButton)
            button->setIcon(QIcon(":/images/publish_hover.png"));
        else if(button == m_previewButton)
            button->setIcon(QIcon(":/images/preview_hover.png"));
    }

    return false;
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
    qDebug() << "create";
}

void Dashboard::publishClicked()
{
    qDebug() << "publish";
}

void Dashboard::previewClicked()
{
    emit previewSite();
}

void Dashboard::siteLoaded(Site *site)
{
    m_site = site;
    m_info->setText(m_site->title() + " loaded...");
}
