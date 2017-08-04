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

TextEditor::TextEditor()
{
    m_changed = false;

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

    QPushButton *save = new QPushButton("Save and Exit");
    QPushButton *cancel = new QPushButton("Cancel");
    QHBoxLayout *hbox = new QHBoxLayout();

    QLabel *titleLabel = new QLabel("Edit Text Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    hbox->addStretch();
    hbox->addWidget(save);
    hbox->addWidget(cancel);
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(m_html, 1, 0);
    grid->addLayout(hbox, 2, 0);
    setLayout(grid);

    connect(save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancel()));
    connect(m_html, SIGNAL(textChanged()), this, SLOT(textChanged()));
}

void TextEditor::textChanged()
{
    m_changed = true;
}

void TextEditor::save()
{
    emit close(this);
}

void TextEditor::cancel()
{
    m_changed = false;
    emit close(this);
}
