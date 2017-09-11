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
#include <QXmlStreamWriter>

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

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 3, 1, 1, Qt::AlignRight);
    grid->addWidget(new QLabel("Path"), 1, 0);
    grid->addWidget(m_source, 2, 0, 1, 3);
    grid->addWidget(seek, 2, 3);
    grid->addWidget(m_image, 3, 0, 1, 4);
    grid->setRowStretch(3, 1);
    grid->addWidget(new QLabel("Alt"), 6, 0);
    grid->addWidget(m_alt, 7, 0);
    grid->addWidget(new QLabel("Title"), 8, 0);
    grid->addWidget(m_title, 9, 0);
    grid->addWidget(new QLabel("Admin Label"), 10, 0);
    grid->addWidget(m_adminlabel, 11, 0);
    setLayout(grid);

    connect(m_source, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_alt, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_image, SIGNAL(clicked()), this, SLOT(seek()));
    connect(seek, SIGNAL(clicked()), this, SLOT(seek()));
    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
}

QString ImageEditor::load(QXmlStreamReader *streamin)
{
    QString content;
    QXmlStreamWriter stream(&content);
    stream.writeStartElement("Image");
    stream.writeAttribute("src", streamin->attributes().value("src").toString());
    stream.writeAttribute("alt", streamin->attributes().value("alt").toString());
    stream.writeAttribute("title", streamin->attributes().value("title").toString());
    stream.writeAttribute("adminlabel", streamin->attributes().value("adminlabel").toString());
    stream.writeEndElement();
    return content;
}

void ImageEditor::setContent(QString content)
{
    m_content = content;
    QXmlStreamReader stream(m_content);
    stream.readNextStartElement();
    QString src = stream.attributes().value("src").toString();
    m_source->setText(src);
    if(!src.isEmpty())
        m_image->setImage(QImage(src));
    else
        m_image->setImage(QImage(":/images/image_placeholder.png"));
    m_alt->setText(stream.attributes().value("alt").toString());
    m_title->setText(stream.attributes().value("title").toString());
    m_adminlabel->setText(stream.attributes().value("adminlabel").toString());
    m_changed = false;
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

void ImageEditor::closeEditor()
{
    if(m_changed)
    {
        m_content = "";
        QXmlStreamWriter stream(&m_content);
        stream.writeStartElement("Image");
        stream.writeAttribute("src", m_source->text());
        stream.writeAttribute("alt", m_alt->text());
        stream.writeAttribute("title", m_title->text());
        stream.writeAttribute("adminlabel", m_adminlabel->text());
        stream.writeEndElement();
    }
    emit close();
}

QString ImageEditor::getHtml(QDomElement ele)
{
    QString source = ele.attribute("src");
    QString url = source.mid(source.indexOf("assets/images/"));
    QString alt = ele.attribute("alt", "");
    QString title = ele.attribute("title", "");

    return "<img alt=\"" + alt + "\" title=\"" + title + "\" class=\"img-responsive pull-left inner\" src=\"" + url + "\">\n";
}
