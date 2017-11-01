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

#include "installdialog.h"
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDir>
#include <QFileDialog>

InstallDialog::InstallDialog()
{
    setWindowTitle("Install FlatSiteBuilder");
    QHBoxLayout *hbox = new QHBoxLayout;
    QVBoxLayout *vbox = new QVBoxLayout;
    QHBoxLayout *pathBox = new QHBoxLayout;
    QGridLayout *layout = new QGridLayout();
    QLabel *icon = new QLabel;
    icon->setPixmap(QPixmap(":/images/icon_128.png"));
    m_path = new QLineEdit;
    m_installDirectory = QDir::homePath() + "/FlatSiteBuilder";
    m_path->setText(m_installDirectory);
    QPushButton *choose = new QPushButton("...");
    QPushButton *install = new QPushButton("Install");
    install->setDefault(true);
    QPushButton *cancel = new QPushButton("Cancel");
    pathBox->addWidget(m_path);
    pathBox->addWidget(choose);
    hbox->addStretch();
    hbox->addWidget(cancel);
    hbox->addWidget(install);
    vbox->addWidget(new QLabel("FlatSiteBuilder is about to be installed onto your computer."));
    vbox->addWidget(new QLabel("Please choose a directory where the files should be stored."));
    vbox->addLayout(pathBox);
    vbox->addStretch();
    layout->addWidget(icon, 0, 0);
    layout->addLayout(vbox, 0, 1);
    layout->addLayout(hbox, 1, 0, 1, 2);
    setLayout(layout);

    connect(install, SIGNAL(clicked(bool)), this, SLOT(installClicked()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancelClicked()));
    connect(choose, SIGNAL(clicked(bool)), this, SLOT(chooseClicked()));
}

void InstallDialog::installClicked()
{
    m_installDirectory = m_path->text();
    close();
}

void InstallDialog::cancelClicked()
{
    m_installDirectory = "";
    close();
}

void InstallDialog::chooseClicked()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::DirectoryOnly);
    dialog->setWindowTitle(tr("Install FlatSiteBuilder to ..."));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setDirectory(m_path->text());
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if (fileName.isEmpty())
        return;
    m_installDirectory = fileName;
    m_path->setText(m_installDirectory);
}
