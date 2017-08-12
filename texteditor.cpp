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
#include <QGridLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QTest>

TextEditor::TextEditor()
    : AbstractEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    m_html = new QTextEdit();
    m_html->setFont(font);
    m_html->setAcceptRichText(false);
    m_html->setLineWrapMode(QTextEdit::NoWrap);
    QFontMetrics metrics(font);
    m_html->setTabStopWidth(4 * metrics.width(' '));
    new HtmlHighlighter(m_html->document());
    m_adminlabel = new QLineEdit();
    m_adminlabel->setMaximumWidth(200);
    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("Text Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(cancel);
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(m_html, 1, 0);
    grid->addWidget(new QLabel("Admin Label"), 2, 0);
    grid->addWidget(m_adminlabel, 3, 0);
    grid->addLayout(hbox, 4, 0);
    setLayout(grid);

    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(m_html, SIGNAL(textChanged()), this, SLOT(contentChanged()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
}

void TextEditor::save()
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
            QDomNode data = m_element.firstChild();
            QDomCDATASection cdata = data.toCDATASection();
            cdata.setData(m_html->toPlainText());
        }
        m_element.setAttribute("adminlabel", m_adminlabel->text());
    }
    emit close(this);
}

void TextEditor::cancel()
{
    m_changed = false;
    emit close(this);
}

void TextEditor::setContent(QDomElement element)
{
    m_element = element;
    m_adminlabel->setText(m_element.attribute("adminlabel", ""));
    QDomNode data = m_element.firstChild();
    QDomCDATASection cdata = data.toCDATASection();
    m_html->setPlainText(cdata.data());
    m_changed = false;
}
