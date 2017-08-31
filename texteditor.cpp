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
#include "htmlhighlighter.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTest>

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

    new HtmlHighlighter(m_html->document());

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
        if(m_element.isNull())
        {
            m_element = m_doc.createElement("Text");
            m_element.appendChild(m_doc.createCDATASection(m_html->toPlainText()));
        }
        else
        {
            if(m_element.nodeName() == "Text")
            {
                QDomNode data = m_element.firstChild();
                QDomCDATASection cdata = data.toCDATASection();
                cdata.setData(m_html->toPlainText());
            }
            else
            {
                m_doc.setContent(m_html->toPlainText());
                m_element = m_doc.documentElement();
            }
        }
        m_element.setAttribute("adminlabel", m_adminlabel->text());
    }
    emit close();
}

void TextEditor::setContent(QDomElement element)
{
    m_element = element;
    if(element.nodeName() == "Text")
    {
        m_adminlabel->setText(m_element.attribute("adminlabel", ""));
        QDomNode data = m_element.firstChild();
        QDomCDATASection cdata = data.toCDATASection();
        m_html->setPlainText(cdata.data());
    }
    else
    {
        QString txt = "<" + element.nodeName();
        QDomNamedNodeMap attrs = element.attributes();
        for(int i = 0; i < attrs.count(); i++)
        {
            QDomAttr att = attrs.item(i).toAttr();
            if(att.name() == "adminlabel")
                m_adminlabel->setText(m_element.attribute("adminlabel", ""));
            else
                txt += QString::fromLatin1(" %0=\"%1\"").arg(att.name()).arg(att.value());
        }
        txt += ">";
        m_html->setPlainText(txt);
    }
    m_changed = false;
}

QString TextEditor::getHtml(QDomElement text, QMap<QString, EditorInterface*>)
{
    QDomNode data = text.firstChild();
    QDomCDATASection cdata = data.toCDATASection();
    return cdata.data();
}
