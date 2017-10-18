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

#include "sectionpropertyeditor.h"
#include "rowpropertyeditor.h"
#include "mainwindow.h"
#include "flatbutton.h"
#include "plugins.h"
#include <QPushButton>
#include <QXmlStreamReader>
#include <QLabel>
#include <QTest>
#include <QStatusBar>

SectionPropertyEditor::SectionPropertyEditor()
{
    m_grid = new QGridLayout();
    m_grid->setMargin(0);
    m_cssclass = new QLineEdit();
    m_style = new QLineEdit();
    m_attributes = new QLineEdit();
    m_id = new QLineEdit;
    m_changed = false;
    setAutoFillBackground(true);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    QLabel *titleLabel = new QLabel("Section Module Settings");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();

    m_grid->addWidget(titleLabel, 0, 0);
    m_grid->addWidget(close, 0, 1, 1, 1, Qt::AlignRight);
    m_grid->addWidget(new QLabel("CSS Class"), 1, 0);
    m_grid->addWidget(m_cssclass, 2, 0, 1, 2);
    m_grid->addWidget(new QLabel("Style"), 3, 0);
    m_grid->addWidget(m_style, 4, 0, 1, 2);
    m_grid->addWidget(new QLabel("Aditional Attributes"), 5, 0);
    m_grid->addWidget(m_attributes, 6, 0, 1, 2);
    m_grid->addWidget(new QLabel("Id"), 7, 0);
    m_grid->addWidget(m_id, 8, 0, 1, 2);
    m_grid->addLayout(vbox, 10, 0);
    setLayout(m_grid);

    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_cssclass, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_style, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_attributes, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(m_id, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
 }

void SectionPropertyEditor::closeEditor()
{
    if(m_changed)
    {
        m_content = "";
        QXmlStreamWriter stream(&m_content);
        stream.writeStartElement("Section");
        stream.writeAttribute("cssclass", m_cssclass->text());
        stream.writeAttribute("style", m_style->text());
        stream.writeAttribute("attributes", m_attributes->text());
        stream.writeAttribute("id", m_id->text());
        stream.writeAttribute("fullwidth", m_fullwidth ? "true" : "false");
        stream.writeEndElement();
    }
    emit close();
}

void SectionPropertyEditor::setContent(QString content)
{
    m_content = content;
    QXmlStreamReader stream(m_content);
    stream.readNextStartElement();
    m_cssclass->setText(stream.attributes().value("cssclass").toString());
    m_style->setText(stream.attributes().value("style").toString());
    m_attributes->setText(stream.attributes().value("attributes").toString());
    m_id->setText(stream.attributes().value("id").toString());
    m_fullwidth = stream.attributes().value("fullwidth").toString() == "true";
    m_changed = false;
}

QString SectionPropertyEditor::getHtml(QXmlStreamReader *xml, QString filename)
{
    QString id = xml->attributes().value("id").toString();
    QString cls = xml->attributes().value("cssclass").toString();
    QString style = xml->attributes().value("style").toString();
    QString attributes = xml->attributes().value("attributes").toString();
    bool fullwidth = xml->attributes().value("fullwidth") == "true";
    QString html = "";

    if(fullwidth)
    {
        while(xml->readNext())
        {
            if(xml->isStartElement())
            {
                QString pluginName = xml->name() + "Editor";
                if(Plugins::hasElementPlugin(pluginName))
                {
                    html += Plugins::getElementPlugin(pluginName)->getHtml(xml);
                    Plugins::addUsedPlugin(pluginName);
                }
                else
                    qDebug() << "Undefined element " + pluginName;
            }
            else if(xml->isEndElement())
                break;
            else if(xml->atEnd())
            {
                if(xml->hasError())
                    qWarning() << xml->errorString() + " in line " + QString::number(xml->lineNumber()) + " column " + QString::number(xml->columnNumber()) + " in file " + filename;
                return "";
            }
        }
    }
    else
    {
        html += "<section";
        if(cls.isEmpty())
            cls = "container";
        html += " class=\"" + cls + "\"";
        if(!id.isEmpty())
            html += " id=\"" + id +"\"";
        if(!style.isEmpty())
            html += " style=\"" + style + "\"";
        if(!attributes.isEmpty())
            html += " " + attributes;
        html += ">\n";

        while(xml->readNext())
        {
            if(xml->isStartElement())
            {
                if(xml->name() == "Row")
                {
                    html += RowPropertyEditor::getHtml(xml, filename);
                    xml->readNext();
                }
                else if(xml->name().isEmpty())
                    ; // ignore
                else
                    qWarning() << "Wrong starttag " + xml->name() + " in line " + QString::number(xml->lineNumber()) + " in file " + filename;
            }
            else if(xml->isEndElement() || xml->atEnd())
                break;
        }
        html += "</section>";
    }
    return html;
}

