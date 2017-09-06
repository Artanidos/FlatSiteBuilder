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
#include <QStandardItemModel>
#include <QStandardPaths>

ImageEditor::ImageEditor()
{
    m_changed = false;
    setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    m_source = new QLineEdit();
    m_alt = new QLineEdit();
    m_alt->setMaximumWidth(200);
    m_title = new QLineEdit();
    m_title->setMaximumWidth(200);
    m_adminlabel = new QLineEdit();
    m_adminlabel->setMaximumWidth(200);
    QPushButton *seek = new QPushButton("...");
    seek->setMaximumWidth(50);
    QLabel *titleLabel = new QLabel("Image Module Plugin");
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

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    m_animationCombo = new QComboBox();
    m_animationCombo->addItem("None", "none");
    addHeader("Attention Seekers");
    m_animationCombo->addItem("Bounce","bounce");
    m_animationCombo->addItem("Flash","flash");
    m_animationCombo->addItem("Pulse","pulse");
    m_animationCombo->addItem("Rubber Band","rubberBand");
    m_animationCombo->addItem("Shake","shake");
    m_animationCombo->addItem("Swing","swing");
    m_animationCombo->addItem("Tada","tada");
    m_animationCombo->addItem("Wobble","wobble");
    m_animationCombo->addItem("Jello","jello");
    addHeader("Bouncing Entrances");
    m_animationCombo->addItem("Bounce In","bounceIn");
    m_animationCombo->addItem("Bounce In Down","bounceInDown");
    m_animationCombo->addItem("Bounce In Left","bounceInLeft");
    m_animationCombo->addItem("Bounce In Right","bounceInRight");
    m_animationCombo->addItem("Bounce In Up","bounceInUp");
    addHeader("Bouncing Exits");
    m_animationCombo->addItem("Bounce Out","bounceOut");
    m_animationCombo->addItem("Bounce Out Down","bounceOutDown");
    m_animationCombo->addItem("Bounce Out Left","bounceOutLeft");
    m_animationCombo->addItem("Bounce Out Right","bounceOutRight");
    m_animationCombo->addItem("Bounce Out Up","bounceOutUp");
    addHeader("Fading Entrances");
    m_animationCombo->addItem("Fade In", "fadeIn");
    m_animationCombo->addItem("Fade In Down", "fadeInDown");
    m_animationCombo->addItem("Fade In Down Big", "fadeInDownBig");
    m_animationCombo->addItem("Fade In Left", "fadeInLeft");
    m_animationCombo->addItem("Fade In Left Big", "fadeInLeftBig");
    m_animationCombo->addItem("Fade In Right", "fadeInRight");
    m_animationCombo->addItem("Fade In Right Big", "fadeInRightBig");
    m_animationCombo->addItem("Fade In Up", "fadeInUp");
    m_animationCombo->addItem("Fade In Up Big", "fadeInUpBig");
    addHeader("Fading Exits");
    m_animationCombo->addItem("Fade Out", "fadeOut");
    m_animationCombo->addItem("Fade Out Down", "fadeOutDown");
    m_animationCombo->addItem("Fade Out Down Big", "fadeOutDownBig");
    m_animationCombo->addItem("Fade Out Left", "fadeOutLeft");
    m_animationCombo->addItem("Fade Out Left Big", "fadeOutLeftBig");
    m_animationCombo->addItem("Fade Out Right", "fadeOutRight");
    m_animationCombo->addItem("Fade Out Right Big", "fadeOutRightBig");
    m_animationCombo->addItem("Fade Out Up", "fadeOutUp");
    m_animationCombo->addItem("Fade Out Up Big", "fadeOutUpBig");
    addHeader("Flippers");
    m_animationCombo->addItem("Flip", "flip");
    m_animationCombo->addItem("Flip In X", "flipInX");
    m_animationCombo->addItem("Flip In Y", "flipInY");
    m_animationCombo->addItem("Flip Out X", "flipOutX");
    m_animationCombo->addItem("Flip Out Y", "flipOutY");
    addHeader("Lightspeed");
    m_animationCombo->addItem("Light Speed In", "lightSpeedIn");
    m_animationCombo->addItem("Light Speed Out", "lightSpeedOut");
    addHeader("Rotating Entrances");
    m_animationCombo->addItem("Rotate In", "rotateIn");
    m_animationCombo->addItem("Rotate In Down Left", "rotateInDownLeft");
    m_animationCombo->addItem("Rotate In Down Right", "rotateInDownRight");
    m_animationCombo->addItem("Rotate In Up Left", "rotateInUpLeft");
    m_animationCombo->addItem("Rotate In Up Right", "rotateInUpRight");
    addHeader("Rotating Exits");
    m_animationCombo->addItem("Rotate Out", "rotateOut");
    m_animationCombo->addItem("Rotate Out Down Left", "rotateOutDownLeft");
    m_animationCombo->addItem("Rotate Out Down Right", "rotateOutDownRight");
    m_animationCombo->addItem("Rotate Out Up Left", "rotateOutUpLeft");
    m_animationCombo->addItem("Rotate Out Up Right", "rotateOutUpRight");
    addHeader("Sliding Entrances");
    m_animationCombo->addItem("Slide In Up", "slideInUp");
    m_animationCombo->addItem("Slide In Down", "slideInDown");
    m_animationCombo->addItem("Slide In Left", "slideInLeft");
    m_animationCombo->addItem("Slide In Right", "slideInRight");
    addHeader("Sliding Exits");
    m_animationCombo->addItem("Slide Out Up", "slideOutUp");
    m_animationCombo->addItem("Slide Out Down", "slideOutDown");
    m_animationCombo->addItem("Slide Out Left", "slideOutLeft");
    m_animationCombo->addItem("Slide Out Right", "slideOutRight");
    addHeader("Zoom Entrances");
    m_animationCombo->addItem("Zoom In", "zoomIn");
    m_animationCombo->addItem("Zoom In Down", "zoomInDown");
    m_animationCombo->addItem("Zoom In Left", "zoomInLeft");
    m_animationCombo->addItem("Zoom In Right", "zoomInRight");
    m_animationCombo->addItem("Zoom In Up", "zoomInUp");
    addHeader("Zoom Exits");
    m_animationCombo->addItem("Zoom Out", "zoomOut");
    m_animationCombo->addItem("Zoom Out Down", "zoomOutDown");
    m_animationCombo->addItem("Zoom Out Left", "zoomOutLeft");
    m_animationCombo->addItem("Zoom Out Right", "zoomOutRight");
    m_animationCombo->addItem("Zoom Out Up", "zoomOutUp");
    addHeader("Specials");
    m_animationCombo->addItem("Hinge", "hinge");
    m_animationCombo->addItem("Jack In The Box", "jackInTheBox");
    m_animationCombo->addItem("Roll In", "rollIn");
    m_animationCombo->addItem("Roll Out", "rollOut");

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2, 1, 1, Qt::AlignRight);
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

void ImageEditor::addHeader(QString header)
{
    m_animationCombo->addItem(header, "");
    QStandardItemModel *model = dynamic_cast<QStandardItemModel*>( m_animationCombo->model());
    model->item(m_animationCombo->count() - 1, 0)->setEnabled(false);
}

void ImageEditor::setContent(QDomElement element)
{
    m_element = element;
    QString src = m_element.attribute("src", "");
    m_source->setText(src);
    if(!src.isEmpty())
        m_image->setImage(QImage(src));
    else
        m_image->setImage(QImage(":/images/image_placeholder.png"));
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

    // copy file to assets dir
    QFileInfo info(fileName);
    QString path = m_site->sourcePath() + "/assets/images/" + info.fileName();
    m_source->setText(path);
    QFile::copy(fileName, path);

    // also copy file to deploy dir for previews
    QString dpath = m_site->deployPath() + "/assets/images/" + info.fileName();
    QFile::copy(fileName, dpath);

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
    emit close();
}

QString ImageEditor::getHtml(QDomElement ele)
{
    QString source = ele.attribute("src");
    QString animation = ele.attribute("animation");
    QString url = source.mid(source.indexOf("assets/images/"));
    QString alt = ele.attribute("alt", "");
    QString title = ele.attribute("title", "");

    if(animation == "none")
        return "<img alt=\"" + alt + "\" title=\"" + title + "\" class=\"img-responsive pull-left inner\" src=\"" + url + "\">\n";
    else
        return "<img alt=\"" + alt + "\" title=\"" + title + "\" class=\"img-responsive animated " + animation + " pull-left inner\" src=\"" + url + "\">\n";
}

QString ImageEditor::pluginStyles()
{
    return "<link href=\"assets/css/animate.css\" rel=\"stylesheet\" type=\"text/css\"/>\n";
}

void ImageEditor::installAssets(QString assetsPath)
{
    QFile tst(assetsPath + "/css/animate.css");
    if(tst.exists())
        tst.remove();
    QFile::copy(":/animate.css", assetsPath + "/css/animate.css");
}
