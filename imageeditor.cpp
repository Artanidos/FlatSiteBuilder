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

#include "imageeditor.h"
#include "imageselector.h"
#include "flatbutton.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QComboBox>
#include <QStandardPaths>
#include <QTest>

ImageEditor::ImageEditor()
    : AbstractEditor()
{
    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    m_source = new QLineEdit();
    m_alt = new QLineEdit();
    m_alt->setMaximumWidth(200);
    m_title = new QLineEdit();
    m_title->setMaximumWidth(200);
    m_adminlabel = new QLineEdit();
    m_adminlabel->setMaximumWidth(200);
    QPushButton *seek = new QPushButton("...");
    seek->setMaximumWidth(50);
    QLabel *titleLabel = new QLabel("Image Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);
    m_image = new ImageSelector();
    m_image->setImage(QImage(":/images/image_placeholder.png"));
    QVBoxLayout *imageVBox = new QVBoxLayout();
    QHBoxLayout *imageHBox = new QHBoxLayout();
    imageHBox->addStretch();
    imageHBox->addWidget(m_image);
    imageHBox->addStretch();
    imageVBox->addStretch();
    imageVBox->addLayout(imageHBox);
    imageVBox->addStretch();

    m_animationCombo = new QComboBox();
    m_animationCombo->addItem("None", "none");
    m_animationCombo->addItem("Fade In", "fadeIn");
    m_animationCombo->addItem("Fade In Up", "fadeInUp");
    m_animationCombo->addItem("Fade In Down","fadeInDown");
    m_animationCombo->addItem("Fade In Left","fadeInLeft");
    m_animationCombo->addItem("Fade In Right","fadeInRight");
    m_animationCombo->addItem("Fade In Up Big","fadeInUpBig");
    m_animationCombo->addItem("Fade In Down Big","fadeInDownBig");
    m_animationCombo->addItem("Fade In Left Big","fadeInLeftBig");
    m_animationCombo->addItem("Fade In Right Big","fadeInRightBig");
    m_animationCombo->addItem("Bounce In","bounceIn");
    m_animationCombo->addItem("Bounce In Up","bounceInUp");
    m_animationCombo->addItem("Bounce In Down","bounceInDown");
    m_animationCombo->addItem("Bounce In Left","bounceInLeft");
    m_animationCombo->addItem("Bounce In Right","bounceInRight");
    m_animationCombo->addItem("Rotate In","rotateIn");
    m_animationCombo->addItem("Rotate In Up Left","rotateInUpLeft");
    m_animationCombo->addItem("Rotate In Down Left","rotateInDownLeft");
    m_animationCombo->addItem("Rotate In Up Right","rotateInUpRight");
    m_animationCombo->addItem("Rotate In Down Right","rotateInDownRight");
    m_animationCombo->addItem("Flash","flash");
    m_animationCombo->addItem("Shake","shake");
    m_animationCombo->addItem("Bounce","bounce");
    m_animationCombo->addItem("Tada","tada");
    m_animationCombo->addItem("Swing","swing");
    m_animationCombo->addItem("Wobble","wobble");
    m_animationCombo->addItem("Wiggle","wiggle");
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2);
    grid->addWidget(new QLabel("Path"), 1, 0);
    grid->addWidget(m_source, 2, 0, 1, 2);
    grid->addWidget(seek, 2, 2);
    grid->addWidget(new QLabel("Animation"), 3, 0);
    grid->addWidget(m_animationCombo, 4, 0);
    grid->addLayout(imageVBox, 5, 0, 1, 3);
    grid->addWidget(new QLabel("Alt"), 6, 0);
    grid->addWidget(m_alt, 7, 0);
    grid->addWidget(new QLabel("Title"), 8, 0);
    grid->addWidget(m_title, 9, 0);
    grid->addWidget(new QLabel("Admin Label"), 10, 0);
    grid->addWidget(m_adminlabel, 11, 0);
    setLayout(grid);

    connect(m_animationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(contentChanged()));
    connect(m_source, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_alt, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_image, SIGNAL(clicked()), this, SLOT(seek()));
    connect(seek, SIGNAL(clicked()), this, SLOT(seek()));
    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
}

void ImageEditor::setContent(QDomElement element)
{
    m_element = element;
    QString src = m_element.attribute("src", "");
    m_source->setText(src);
    if(!src.isEmpty())
        m_image->setImage(QImage(src));
    QString anim = m_element.attribute("animation", "none");
    int index = m_animationCombo->findData(anim, Qt::UserRole);
    m_animationCombo->setCurrentIndex(index);
    m_alt->setText(m_element.attribute("alt", ""));
    m_title->setText(m_element.attribute("title", ""));
    m_adminlabel->setText(m_element.attribute("adminlabel", ""));
}

void ImageEditor::seek()
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

    QFileInfo info(fileName);
    QString path = m_site->path() + "/assets/images/" + info.fileName();
    m_source->setText(path);
    QFile::copy(fileName, path);
    m_image->setImage(QImage(path));
    contentChanged();
}

void ImageEditor::closeEditor()
{
    if(m_changed)
    {
        if(m_element.isNull())
        {
            m_element = m_doc.createElement("Image");
        }
        m_element.setAttribute("src", m_source->text());
        m_element.setAttribute("animation", m_animationCombo->currentData(Qt::UserRole).toString());
        m_element.setAttribute("alt", m_alt->text());
        m_element.setAttribute("title", m_title->text());
        m_element.setAttribute("adminlabel", m_adminlabel->text());
    }
    emit close(this);
}
