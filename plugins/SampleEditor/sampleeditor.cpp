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

#include "sampleeditor.h"
#include "flatbutton.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

SampleEditor::SampleEditor()
{
    m_changed = false;
    setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    m_sampleproperty = new QLineEdit;
    m_adminlabel = new QLineEdit;
    m_adminlabel->setMaximumWidth(200);
    QLabel *titleLabel = new QLabel("Sample Module Plugin");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2, 1, 1, Qt::AlignRight);
    grid->addWidget(new QLabel("Sample Property"), 1, 0);
    grid->addWidget(m_sampleproperty, 2, 0, 1, 3);
    grid->addLayout(vbox, 3, 0); // for stretching only
    grid->addWidget(new QLabel("Admin Label"), 10, 0);
    grid->addWidget(m_adminlabel, 11, 0);
    setLayout(grid);

    connect(m_sampleproperty, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
}

void SampleEditor::setContent(QString content)
{
    m_content = content;
    QXmlStreamReader stream(m_content);
    stream.readNextStartElement();
    m_sampleproperty->setText(stream.attributes().value("sampleproperty").toString());
    m_adminlabel->setText(stream.attributes().value("adminlabel").toString());
}

QString SampleEditor::load(QXmlStreamReader *streamin)
{
    QString content;
    QXmlStreamWriter stream(&content);
    stream.writeStartElement("Sample");
    stream.writeAttribute("sampleproperty", streamin->attributes().value("sampleproperty").toString());
    stream.writeAttribute("adminlabel", streamin->attributes().value("adminlabel").toString());
    stream.writeEndElement();
    return content;
}

void SampleEditor::closeEditor()
{
    if(m_changed)
    {
        QXmlStreamWriter stream(&m_content);
        stream.writeStartElement("Sample");
        stream.writeAttribute("sampleproperty", m_sampleproperty->text());
        stream.writeAttribute("adminlabel", m_adminlabel->text());
        stream.writeEndElement();
    }
    emit close();
}

QString SampleEditor::getHtml(QDomElement ele)
{
    QString sampleproperty = ele.attribute("sampleproperty", "");
    return "<div sampleproperty=\"" + sampleproperty + "\">\n";
}
