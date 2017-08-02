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
#include "hyperlink.h"
#include "texteditor.h"
#include "animationlabel.h"
#include "pageeditor.h"
#include "sectioneditor.h"
#include "roweditor.h"
#include "elementeditor.h"
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDomDocument>
#include <QDomElement>
#include <QTest>

ContentEditor::ContentEditor(Site *site, Content *content)
{
    m_site = site;
    m_content = content;

    QString txt = "preview ";
    if(m_content->contentType() == ContentType::Page)
        txt += "page";
    else
        txt += "post";
    m_previewLink = new Hyperlink(txt);
    m_vbox = new QVBoxLayout();
    m_layout = new QGridLayout();
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
    m_excerpt = new QLineEdit();

    m_scroll = new QScrollArea();
    //PageEditor *pe = new PageEditor();
    //SectionEditor *se = new SectionEditor();
    //RowEditor *re = new RowEditor();
    //se->addRow(re);
    //pe->addSection(se);
    //m_scroll->setWidget(pe);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll->setWidgetResizable(true);

    m_layout->addWidget(m_titleLabel, 0, 0);
    m_layout->addWidget(m_previewLink, 0, 1);
    m_layout->addWidget(m_title, 1, 0, 1, 2);
    m_layout->addWidget(m_save, 1, 2);
    m_layout->addWidget(m_scroll, 2, 0, 1, 3);
    m_vbox->addLayout(m_layout);
    setLayout(m_vbox);

    m_title->setText(m_content->title());
    if(m_content->contentType() == ContentType::Page)
        m_filename = m_site->path() + "/pages/" + m_content->source();
    else
    {
        m_excerptLabel = new QLabel("Excerpt");
        m_layout->addWidget(m_excerptLabel, 3, 0);
        m_layout->addWidget(m_excerpt, 4, 0, 1, 3);
        m_excerpt->setText(m_content->excerpt());
        m_filename = m_site->path() + "/posts/" + m_content->source();
    }

    load();

    connect(m_save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(editChanged()));
    connect(m_excerpt, SIGNAL(textChanged(QString)), this, SLOT(editChanged()));
    connect(m_previewLink, SIGNAL(linkActivated(QString)), this, SLOT(preview()));
}

void ContentEditor::load()
{
    QDomDocument doc;
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open " + m_filename;
        return;
    }
    if (!doc.setContent(&file))
    {
        qDebug() << "Unable to the post content from XML";
        file.close();
        return;
    }
    file.close();

    PageEditor *pe = new PageEditor();
    m_scroll->setWidget(pe);
    QDomElement post = doc.documentElement();
    QDomElement section = post.firstChildElement("Section");
    while(!section.isNull())
    {
        SectionEditor *se = new SectionEditor();
        pe->addSection(se);
        loadRows(section, se);
        section = section.nextSiblingElement("Section");
    }
}

void ContentEditor::loadRows(QDomElement section, SectionEditor *se)
{
    QDomElement row = section.firstChildElement("Row");
    while(!row.isNull())
    {
        RowEditor *re = new RowEditor();
        se->addRow(re);
        loadColumns(row, re);
        row = row.nextSiblingElement("Row");
    }
}

void ContentEditor::loadColumns(QDomElement row, RowEditor *re)
{
    int i = 0;
    QDomElement column = row.firstChildElement("Column");
    while(!column.isNull())
    {
        ColumnEditor *ce = new ColumnEditor();
        re->addColumn(ce, i, column.attribute("span", "1").toInt());
        loadElements(column, ce);
        column = column.nextSiblingElement("Column");
        i++;
    }
}

void ContentEditor::loadElements(QDomElement column, ColumnEditor *ce)
{
    QDomElement element = column.firstChildElement();
    while(!element.isNull())
    {
        ElementEditor *ee = new ElementEditor();
        ee->setText(element.nodeName());
        ee->setMode(ElementEditor::Mode::Enabled);
        QDomNode data = element.firstChild();
        QDomCDATASection cdata = data.toCDATASection();
        ee->setContent(cdata.data());
        ce->addElement(ee);
        element = element.nextSiblingElement();
    }
}

void ContentEditor::save()
{
    if(m_content->source().isEmpty())
    {
        if(m_content->contentType() == ContentType::Page)
        {
            m_content->setLayout("default");
            m_filename = m_site->path() + "/pages/" + m_title->text().toLower() + ".html";
        }
        else // TODO: real date here
        {
            m_content->setLayout("post");
            m_filename = m_site->path() + "/posts/" + m_title->text().toLower() + ".html";
        }
        // TODO: real author here
        m_content->setAuthor("Olaf Japp");
        m_content->setSource(m_title->text().toLower() + ".html");
        m_content->setDate(QDate());   
        m_site->addContent(m_content);
    }

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

    if(m_content->contentType() == ContentType::Post)
        m_content->setExcerpt(m_excerpt->text());
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

void ContentEditor::preview()
{
    save();
    emit preview(m_content);
}

void ContentEditor::elementEdit(ElementEditor *ee)
{
    QPoint pos = ee->mapTo(m_scroll, ee->pos());

    // make screenprint from elementeditor
    QPixmap pixmapEe(ee->size());
    ee->render(&pixmapEe);

    // make screenprint from scrollarea
    QPixmap pixmapScroll(m_scroll->size());
    m_scroll->render(&pixmapScroll);

    m_editor = new TextEditor();
    m_editor->setContent(ee->content());
    connect(m_editor, SIGNAL(close(QWidget*)), this, SLOT(editorClose(QWidget*)));

    m_animationPanel = new QWidget();
    QLabel *scroll = new QLabel();
    scroll->setPixmap(pixmapScroll);
    scroll->setParent(m_animationPanel);
    scroll->move(0, 0);
    AnimationLabel *anim = new AnimationLabel();
    anim->setPixmap(pixmapEe);
    anim->setAlignment(Qt::AlignTop);
    anim->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, palette().base().color());
    anim->setPalette(pal);
    anim->setParent(m_animationPanel);
    m_animationPanel->setMinimumWidth(m_scroll->size().width());
    m_animationPanel->setMinimumHeight(m_scroll->size().height());
    m_layout->replaceWidget(m_scroll, m_animationPanel);
    m_scroll->hide();

    m_animationgroup = new QParallelAnimationGroup();
    QPropertyAnimation *animx = new QPropertyAnimation();
    animx->setDuration(300);
    animx->setStartValue(pos.x());
    animx->setEndValue(0);
    animx->setTargetObject(anim);
    animx->setPropertyName("x");
    m_animationgroup->addAnimation(animx);
    QPropertyAnimation *animy = new QPropertyAnimation();
    animy->setDuration(300);
    animy->setStartValue(pos.y());
    animy->setEndValue(0);
    animy->setTargetObject(anim);
    animy->setPropertyName("y");
    m_animationgroup->addAnimation(animy);
    QPropertyAnimation *animw = new QPropertyAnimation();
    animw->setDuration(300);
    animw->setStartValue(ee->size().width());
    animw->setEndValue(m_scroll->size().width());
    animw->setTargetObject(anim);
    animw->setPropertyName("width");
    m_animationgroup->addAnimation(animw);
    QPropertyAnimation *animh = new QPropertyAnimation();
    animh->setDuration(300);
    animh->setStartValue(ee->size().height());
    animh->setEndValue(m_scroll->size().height());
    animh->setTargetObject(anim);
    animh->setPropertyName("height");
    m_animationgroup->addAnimation(animh);
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    m_animationgroup->start();
}

void ContentEditor::animationFineshedZoomIn()
{
    m_layout->replaceWidget(m_animationPanel, m_editor);
    m_animationPanel->hide();
    m_title->hide();
    m_save->hide();
    m_previewLink->hide();
    if(m_content->contentType() == ContentType::Post)
    {
        m_excerptLabel->hide();
        m_excerpt->hide();
    }
}

void ContentEditor::editorClose(QWidget *w)
{
    m_title->show();
    m_save->show();
    m_previewLink->show();
    if(m_content->contentType() == ContentType::Post)
    {
        m_excerptLabel->show();
        m_excerpt->show();
    }
    m_layout->replaceWidget(w, m_animationPanel);
    m_animationPanel->show();
    delete w;
    m_animationgroup->setDirection(QAbstractAnimation::Backward);
    disconnect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomOut()));
    m_animationgroup->start();
}

void ContentEditor::animationFineshedZoomOut()
{
    m_layout->replaceWidget(m_animationPanel, m_scroll);
    m_animationPanel->hide();
    m_scroll->show();
    delete m_animationPanel;
    delete m_animationgroup;
}
