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

#include "contenteditor.h"
#include "htmlhighlighter.h"
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QTest>

ContentEditor::ContentEditor(Site *site, Content *content)
{
    m_site = site;
    m_content = content;

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    QVBoxLayout *vbox = new QVBoxLayout();
    QGridLayout *layout = new QGridLayout();
    m_save = new QPushButton();
    m_save->setText(m_content ? "Update" : "Save");
    m_save->setEnabled(false);
    m_save->setMaximumWidth(120);
    m_titleLabel = new QLabel();
    if(!m_content->title().isEmpty())
        m_titleLabel->setText(m_content->contentType() == ContentType::Page ? "Edit Page" : "Edit Post");
    else
        m_titleLabel->setText(m_content->contentType() == ContentType::Page ? "Add new Page" : "Add new Post");
    QFont fnt = m_titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    m_titleLabel->setFont(fnt);
    m_title = new QLineEdit();
    m_text = new QTextEdit;
    m_text->setFont(font);
    QFontMetrics metrics(font);
    m_text->setTabStopWidth(4 * metrics.width(' '));
    new HtmlHighlighter(m_text->document());
    layout->addWidget(m_titleLabel, 0, 0);
    layout->addWidget(m_title, 1, 0, 1, 2);
    layout->addWidget(m_save, 1, 2);
    layout->addWidget(m_text, 2, 0, 1, 3);
    vbox->addLayout(layout);
    setLayout(vbox);

    m_title->setText(m_content->title());
    m_filename = m_site->path() + "/" + m_content->source();
    if(!m_content->source().isEmpty())
    {
        QFile file(m_filename);
        if(file.open(QFile::ReadOnly))
        {
            QByteArray c = file.readAll();
            m_text->insertPlainText(QString::fromLatin1(c));
            file.close();
        }
        else
            qDebug() << "Unable to open file " + m_filename;
    }
    connect(m_save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(editChanged()));
    connect(m_text, SIGNAL(textChanged()), this, SLOT(editChanged()));
}

void ContentEditor::save()
{
    if(m_content->source().isEmpty())
    {
        if(m_content->contentType() == ContentType::Page)
        {
            m_content->setSource(m_title->text().toLower() + ".html");
            m_content->setLayout("default");
        }
        else // TODO: real date here
        {
            m_content->setSource("posts/2017/july/" + m_title->text().toLower() + ".html");
            m_content->setLayout("post");
        }
        // TODO: real author here
        m_content->setAuthor("Olaf Japp");
        m_content->setDate(QDate());
        m_site->addContent(m_content);
    }
    m_filename = m_site->path() + "/" + m_content->source();
    QFile file(m_filename);
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + m_filename;
        return;
    }
    qint64 bytes = file.write(m_text->toPlainText().toLatin1());
    if(bytes == -1)
    {
        file.close();
        qDebug() << "An error occured while writing to file " + m_filename;
        return;
    }
    file.close();

    m_content->setDate(QDate::currentDate());
    m_content->setTitle(m_title->text());

    m_save->setText("Update");
    m_save->setEnabled(false);
    if(m_content->contentType() == ContentType::Page)
        m_titleLabel->setText("Edit Page");
    else
        m_titleLabel->setText("Edit Post");

    emit contentUpdated();
}

void ContentEditor::editChanged()
{
    m_save->setEnabled(true);
}
