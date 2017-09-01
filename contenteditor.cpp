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
#include "commands.h"
#include "sectioneditor.h"
#include "roweditor.h"
#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>
#include <QTemporaryFile>
#include <QTextEdit>
#include <QLineEdit>
#include <QScrollArea>
#include <QMenu>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDomDocument>
#include <QDomElement>
#include <QApplication>
#include <QAction>
#include <QTest>

ContentEditor::ContentEditor(MainWindow *win, Site *site, Content *content)
{
    m_site = site;
    m_win = win;
    m_content = content;
    m_editor = NULL;
    m_undoStack = new QUndoStack;
    m_changed = false;
    setAutoFillBackground(true);

    setContextMenuPolicy(Qt::CustomContextMenu);

    QString txt = "view ";
    if(m_content->contentType() == ContentType::Page)
        txt += "page";
    else
        txt += "post";
    m_previewLink = new Hyperlink(txt);
    m_vbox = new QVBoxLayout();
    m_layout = new QGridLayout();
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
    m_source = new QLineEdit;
    m_excerpt = new QLineEdit();
    m_labelPermalink = new QLabel("Permalink");
    m_labelTitle = new QLabel("Title");

    m_close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    m_close->setToolTip("Close Content Editor");
    m_undo = new FlatButton(":/images/undo_normal.png", ":/images/undo_hover.png", "", ":/images/undo_disabled.png");
    m_redo = new FlatButton(":/images/redo_normal.png", ":/images/redo_hover.png", "", ":/images/redo_disabled.png");
    m_undo->setToolTip("Undo");
    m_redo->setToolTip("Redo");
    m_undo->setEnabled(false);
    m_redo->setEnabled(false);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(m_undo);
    hbox->addWidget(m_redo);
    hbox->addWidget(m_close);
    m_scroll = new QScrollArea();
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll->setWidgetResizable(true);
    m_scroll->installEventFilter(this);

    m_layout->addWidget(m_titleLabel, 0, 0);
    m_layout->addWidget(m_previewLink, 0, 1);
    m_layout->addLayout(hbox, 0, 2);
    m_layout->addWidget(m_labelTitle, 1, 0);
    m_layout->addWidget(m_title, 2, 0);
    m_layout->addWidget(m_labelPermalink, 1, 1);
    m_layout->addWidget(m_source, 2, 1);
    m_layout->addWidget(m_scroll, 5, 0, 1, 3);
    m_vbox->addLayout(m_layout);
    setLayout(m_vbox);

    m_title->setText(m_content->title());
    m_source->setText(m_content->source());
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

    if(m_content->source().isEmpty())
        init();
    else
        load();

    connect(m_close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(m_redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(m_title, SIGNAL(editingFinished()), this, SLOT(titleChanged()));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(titleChanged(QString)));
    connect(m_source, SIGNAL(editingFinished()), this, SLOT(sourceChanged()));
    connect(m_excerpt, SIGNAL(editingFinished()), this, SLOT(excerptChanged()));
    connect(m_previewLink, SIGNAL(clicked()), this, SLOT(preview()));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showContextMenu(const QPoint &)));
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SLOT(canUndoChanged(bool)));
    connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SLOT(canRedoChanged(bool)));
    connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
    connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SLOT(redoTextChanged(QString)));
}

ContentEditor::~ContentEditor()
{
    delete m_undoStack;
}

void ContentEditor::canUndoChanged(bool can)
{
    m_undo->setEnabled(can);
}

void ContentEditor::canRedoChanged(bool can)
{
    m_redo->setEnabled(can);
}

void ContentEditor::undoTextChanged(QString text)
{
    m_undo->setToolTip("Undo " + text);
}

void ContentEditor::redoTextChanged(QString text)
{
    m_redo->setToolTip("Redo " + text);
}

void ContentEditor::undo()
{
    m_undoStack->undo();
}

void ContentEditor::redo()
{
    m_undoStack->redo();
}

void ContentEditor::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu("Context menu", this);
    QAction *undoAct = m_undoStack->createUndoAction(this, tr("&Undo"));
    undoAct->setShortcuts(QKeySequence::Undo);
    QAction *redoAct = m_undoStack->createRedoAction(this, tr("&Redo"));
    redoAct->setShortcuts(QKeySequence::Redo);
    contextMenu.addAction(undoAct);
    contextMenu.addAction(redoAct);
    contextMenu.exec(mapToGlobal(pos));
}

void ContentEditor::updateNewContent()
{
    m_isNew = false;
    if(m_title->text().isEmpty())
    {
        m_content->setTitle("New");
        m_title->setText("New");
    }
    else
        m_content->setTitle(m_title->text());

    if(m_source->text().isEmpty())
    {
        m_content->setSource("new.xml");
        m_source->setText("new.xml");
    }
    else
        m_content->setSource(m_source->text());

    if(m_content->contentType() == ContentType::Page)
    {
        m_content->setLayout("default");
        m_filename = m_site->path() + "/pages/" + m_content->source();
    }
    else
    {
        m_content->setLayout("post");
        m_filename = m_site->path() + "/posts/" + m_content->source();
    }
    m_content->setMenu("default");

    // TODO: real author here
    m_content->setAuthor("author");
    m_content->setDate(QDate::currentDate());
    save();
    emit contentChanged(m_content);
    emit contentUpdated("Content updated");
}

void ContentEditor::closeEditor()
{
    if(m_isNew)
        updateNewContent();

    emit contentEditorClosed();
}

void ContentEditor::siteLoaded(Site *site)
{
    m_site = site;
    foreach(Content *c, m_site->contents())
    {
        if(c->source() == m_content->source() && c->contentType() == m_content->contentType())
        {
            m_excerpt->setText(c->excerpt());
            m_title->setText(c->title());
        }
    }
}

void ContentEditor::titleChanged(QString title)
{
    if(m_isNew)
    {
        QString source = title.toLower() + ".xml";
        m_content->setSource(source);
        m_source->setText(source);
    }
}

void ContentEditor::titleChanged()
{
    if(m_title->text() != m_content->title())
    {
        m_content->setDate(QDate::currentDate());
        m_content->setTitle(m_title->text());
        emit contentChanged(m_content);
        if(!m_isNew)
            emit contentUpdated("Titel Changed");
    }
}

void ContentEditor::sourceChanged()
{
    if(m_source->text() != m_content->source())
    {
        m_content->setDate(QDate::currentDate());
        m_content->setSource(m_source->text());
        if(m_content->contentType() == ContentType::Page)
            m_filename = m_site->path() + "/pages/" + m_content->source();
        else
            m_filename = m_site->path() + "/pages/" + m_content->source();
        emit contentChanged(m_content);
        emit contentUpdated("Permalink Changed");
    }
}

void ContentEditor::excerptChanged()
{
    if(m_excerpt->text() != m_content->excerpt())
    {
        m_content->setDate(QDate::currentDate());
        m_content->setExcerpt(m_excerpt->text());
        emit contentChanged(m_content);
        emit contentUpdated("Excerpt Changed");
    }
}

void ContentEditor::init()
{
    m_isNew = true;
    PageEditor *pe = new PageEditor();
    m_scroll->setWidget(pe);
}

void ContentEditor::load()
{
    m_isNew = false;
    QDomDocument doc;
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        m_statusbar->showMessage("ContentEditor::load(): Unable to open " + m_filename);
        return;
    }
    if (!doc.setContent(&file))
    {
        m_statusbar->showMessage("ContentEditor::load(): Unable to read the post content from XML");
        file.close();
        return;
    }
    file.close();

    PageEditor *pe = new PageEditor();
    m_scroll->setWidget(pe);
    QDomElement content = doc.documentElement();
    QDomElement section = content.firstChildElement("Section");
    while(!section.isNull())
    {
        QString fw = section.attribute("fullwidth", "false");
        SectionEditor *se = new SectionEditor(fw == "true");
        se->setCssClass(section.attribute("cssclass"));
        se->setStyle(section.attribute("style"));
        se->setAttributes(section.attribute("attributes"));
        se->setId(section.attribute("id"));
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
        re->setCssClass(row.attribute("cssclass"));
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
        ce->setSpan(column.attribute("span", "1").toInt());
        re->addColumn(ce, i);
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
        ee->setMode(ElementEditor::Mode::Enabled);
        ee->setContent(element);
        ce->addElement(ee);
        element = element.nextSiblingElement();
    }
}

void ContentEditor::save()
{
    QFile file(m_filename);
    if(!file.open(QFile::WriteOnly))
    {
        m_statusbar->showMessage("ContentEditor::save(): Unable to open file " + m_filename);
        return;
    }
    QDomDocument doc;
    QDomElement root;
    root = doc.createElement("Content");
    doc.appendChild(root);
    PageEditor *pe = dynamic_cast<PageEditor*>(m_scroll->widget());
    foreach(SectionEditor *se, pe->sections())
    {
        se->save(doc, root);
    }
    QTextStream stream(&file);
    stream << doc.toString();
    file.close();
}

void ContentEditor::editChanged(QString text)
{
    if(m_isNew)
        updateNewContent();

    QUndoCommand *changeCommand = new ChangeContentCommand(m_win, this, text);
    m_undoStack->push(changeCommand);
}

void ContentEditor::preview()
{
    emit preview(m_content);
}

bool ContentEditor::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_scroll && event->type() == QEvent::Resize && m_editor)
    {
        m_editor->resize(m_scroll->size());
    }
    return false;
}

void ContentEditor::sectionEdit(SectionEditor *se)
{
    m_sectionEditor = se;

    m_editor = dynamic_cast<EditorInterface*>(m_plugins["SectionPropertyEditor"]);
    m_editor->setContent(se->content());
    connect(m_editor, SIGNAL(close()), this, SLOT(sectionEditorClose()));
    animate(se);
}

void ContentEditor::rowEdit(RowEditor *re)
{
    m_rowEditor = re;

    m_editor = dynamic_cast<EditorInterface*>(m_plugins["RowPropertyEditor"]);
    m_editor->setContent(re->content());
    connect(m_editor, SIGNAL(close()), this, SLOT(rowEditorClose()));

    animate(re);
}

void ContentEditor::elementEdit(ElementEditor *ee)
{
    m_elementEditor = ee;
    if(m_plugins.contains(ee->type()))
        m_editor = dynamic_cast<EditorInterface*>(m_plugins[ee->type()]);
    else
    {
        m_editor = dynamic_cast<EditorInterface*>(m_plugins["TextEditor"]);
        qDebug() << "Plugin for type " + ee->type() + " not loaded.";
    }
    m_editor->setSite(m_site);
    m_editor->setContent(ee->content());
    connect(m_editor, SIGNAL(close()), this, SLOT(editorClose()));

    animate(ee);
}

void ContentEditor::animate(QWidget *widget)
{
    m_sourcewidget = widget;
    QPoint pos = widget->mapTo(m_scroll, QPoint(0,0));

    m_editor->setParent(m_scroll);
    m_editor->move(pos);
    m_editor->resize(widget->size());
    m_editor->show();

    m_animationgroup = new QParallelAnimationGroup();
    m_animx = new QPropertyAnimation();
    m_animx->setDuration(300);
    m_animx->setStartValue(pos.x());
    m_animx->setEndValue(0);
    m_animx->setTargetObject(m_editor);
    m_animx->setPropertyName("x");
    m_animationgroup->addAnimation(m_animx);
    m_animy = new QPropertyAnimation();
    m_animy->setDuration(300);
    m_animy->setStartValue(pos.y());
    m_animy->setEndValue(0);
    m_animy->setTargetObject(m_editor);
    m_animy->setPropertyName("y");
    m_animationgroup->addAnimation(m_animy);
    m_animw = new QPropertyAnimation();
    m_animw->setDuration(300);
    m_animw->setStartValue(widget->size().width());
    m_animw->setEndValue(m_scroll->size().width());
    m_animw->setTargetObject(m_editor);
    m_animw->setPropertyName("width");
    m_animationgroup->addAnimation(m_animw);
    m_animh = new QPropertyAnimation();
    m_animh->setDuration(300);
    m_animh->setStartValue(widget->size().height());
    m_animh->setEndValue(m_scroll->size().height());
    m_animh->setTargetObject(m_editor);
    m_animh->setPropertyName("height");
    m_animationgroup->addAnimation(m_animh);
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    m_animationgroup->start();
}

void ContentEditor::animationFineshedZoomIn()
{
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_title->setEnabled(false);
    m_previewLink->hide();
    m_source->setEnabled(false);
    if(m_content->contentType() == ContentType::Post)
        m_excerpt->setEnabled(false);
}

void ContentEditor::editorClosed()
{   
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QPoint pos = m_sourcewidget->mapTo(m_scroll, QPoint(0,0));
    // correct end values in case of resizing the window
    m_animx->setStartValue(pos.x());
    m_animy->setStartValue(pos.y());
    m_animw->setStartValue(m_sourcewidget->size().width());
    m_animh->setStartValue(m_sourcewidget->size().height());
    m_animationgroup->setDirection(QAbstractAnimation::Backward);
    disconnect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomOut()));
    m_animationgroup->start();
}

void ContentEditor::sectionEditorClose()
{
    if(m_editor->changed())
    {
        m_sectionEditor->setContent(m_editor->content());
        editChanged("Update Section");
    }
    editorClosed();
}

void ContentEditor::rowEditorClose()
{
    if(m_editor->changed())
    {
        m_rowEditor->setContent(m_editor->content());
        editChanged("Update Row");
    }
    editorClosed();
}

void ContentEditor::editorClose()
{
    if(m_editor->changed())
    {
        m_elementEditor->setContent(m_editor->content());
        editChanged("Update Element");
    }
    editorClosed();
}

void ContentEditor::animationFineshedZoomOut()
{
    m_title->setEnabled(true);
    m_source->setEnabled(true);
    m_previewLink->show();
    if(m_content->contentType() == ContentType::Post)
        m_excerpt->setEnabled(true);
    delete m_animationgroup;
    m_editor->hide();
    // do not delete m_editor here, because its a preloaded plugin
    m_editor = NULL;
}
