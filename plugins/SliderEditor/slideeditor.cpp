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

#include "slideeditor.h"
#include "flatbutton.h"
#include "imageselector.h"
#include "slidereditor.h"
#include <QLineEdit>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPushButton>
#include <QGridLayout>

SlideEditor::SlideEditor()
{
    m_changed = false;
    setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    QPushButton *seek = new QPushButton("...");
    seek->setMaximumWidth(50);
    m_adminlabel = new QLineEdit;
    m_adminlabel->setMaximumWidth(200);
    QLabel *titleLabel = new QLabel("Slide");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    m_source = new QLineEdit();
    m_image = new ImageSelector();
    m_image->setImage(QImage(":/images/image_placeholder.png"));

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2, 1, 1, Qt::AlignRight);
    grid->addWidget(new QLabel("Path"), 1, 0);
    grid->addWidget(m_source, 2, 0, 1, 2);
    grid->addWidget(seek, 2, 2);
    grid->addWidget(m_image, 3, 0, 1, 2);
    grid->setRowStretch(3, 1);
    grid->addWidget(new QLabel("Admin Label"), 4, 0);
    grid->addWidget(m_adminlabel, 5, 0);
    setLayout(grid);

    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_image, SIGNAL(clicked()), this, SLOT(seek()));
    connect(seek, SIGNAL(clicked(bool)), this, SLOT(seek()));
    connect(m_source, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
}

void SlideEditor::setSlide(Slide *slide)
{
    m_slide = slide;
    m_source->setText(slide->source());
    if(!slide->source().isEmpty())
        m_image->setImage(QImage(slide->source()));
    else
        m_image->setImage(QImage(":/images/image_placeholder.png"));
    m_adminlabel->setText(slide->adminLabel());
}

void SlideEditor::closeEditor()
{
    if(m_changed)
    {
        m_slide->setSource(m_source->text());
        m_slide->setAdminLabel(m_adminlabel->text());
    }
    emit close();
}

void SlideEditor::seek()
{
    QString fileName;
    QFileDialog *dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setNameFilter(tr("Images (*.png *.gif *.jpg);;All (*)"));
    dialog->setWindowTitle(tr("Load Image"));
    dialog->setOption(QFileDialog::DontUseNativeDialog, true);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setDirectory(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    if(dialog->exec())
        fileName = dialog->selectedFiles().first();
    delete dialog;
    if (fileName.isEmpty())
        return;


    // copy file to assets dir
    QFileInfo info(fileName);
    QString name = info.fileName().replace(" ", "_");
    QString path = m_site->sourcePath() + "/assets/images/" + name;
    m_source->setText(path);
    QFile::copy(fileName, path);

    // also copy file to deploy dir for previews
    QString dpath = m_site->deployPath() + "/assets/images/" + name;
    QFile::copy(fileName, dpath);

    m_image->setImage(QImage(path));
    contentChanged();
}


