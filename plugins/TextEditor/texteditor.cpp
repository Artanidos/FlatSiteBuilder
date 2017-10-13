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

#include "texteditor.h"
#include "flatbutton.h"
#include "xmlhighlighter.h"
#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

TextEditor::TextEditor()
{
    m_changed = false;
    setAutoFillBackground(true);
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");
    m_html = new QTextEdit();
    m_html->setFont(font);
    m_html->setAcceptRichText(false);
    m_html->setLineWrapMode(QTextEdit::NoWrap);
    QFontMetrics metrics(font);
    m_html->setTabStopWidth(4 * metrics.width(' '));

    new XmlHighlighter(m_html->document());

    m_adminlabel = new QLineEdit();
    m_adminlabel->setMaximumWidth(200);

    QLabel *titleLabel = new QLabel("Text Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 1, 1, 1, Qt::AlignRight);
    grid->addWidget(m_html, 1, 0, 1, 2);
    grid->addWidget(new QLabel("Admin Label"), 2, 0);
    grid->addWidget(m_adminlabel, 3, 0, 1, 2);
    setLayout(grid);

    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_html, SIGNAL(textChanged()), this, SLOT(contentChanged()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
}

void TextEditor::closeEditor()
{
    if(m_changed)
    {
        m_content = "";
        QXmlStreamWriter stream(&m_content);
        stream.writeStartElement("Text");
        stream.writeAttribute("adminlabel", m_adminlabel->text());
        stream.writeCDATA(m_html->toPlainText());
        stream.writeEndElement();
    }
    emit close();
}

QString TextEditor::load(QXmlStreamReader *xml)
{
    QString content = "";
    QXmlStreamWriter stream(&content);
    stream.writeStartElement("Text");
    stream.writeAttribute("adminlabel", xml->attributes().value("adminlabel").toString());
    stream.writeCDATA(xml->readElementText());
    stream.writeEndElement();
    return content;
}

void TextEditor::setContent(QString content)
{
    m_content = content;

    QXmlStreamReader stream(m_content);
    stream.readNextStartElement();
    if(stream.name() == "Text")
    {
        m_adminlabel->setText(stream.attributes().value("adminlabel").toString());
        m_html->setPlainText(stream.readElementText());
    }
    else
    {
        m_html->setPlainText(m_content);
        m_adminlabel->setText(stream.attributes().value("adminlabel").toString());
    }
    m_changed = false;
}

QString TextEditor::getHtml(QXmlStreamReader *xml)
{
    return xml->readElementText();
}
