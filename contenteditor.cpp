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
#include "hyperlink.h"
#include "generator.h"
#include "plugins.h"
#include "mainwindow.h"
#include "pageeditor.h"
#include "commands.h"
#include "sectioneditor.h"
#include "roweditor.h"
#include "menu.h"
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
#include <QApplication>
#include <QAction>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTest>

ContentEditor::ContentEditor(MainWindow *win, Site *site, Content *content)
{
    m_win = win;
    m_site = site;
    m_content = content;
    m_editor = NULL;
    m_undoStack = new QUndoStack;
    m_changed = false;
    setAutoFillBackground(true);

    QString txt = "view ";
    if(m_filename.contains("/posts/"))
        txt += "post";
    else
        txt += "page";
    m_previewLink = new Hyperlink(txt);
    m_vbox = new QVBoxLayout();
    m_layout = new QGridLayout();
    m_titleLabel = new QLabel();

    QFont fnt = m_titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    m_titleLabel->setFont(fnt);
    m_title = new QLineEdit;
    m_source = new QLineEdit;
    m_source->setPlaceholderText("*.xml");
    m_excerpt = new QLineEdit;
    m_date = new QLineEdit;
    m_labelPermalink = new QLabel("Permalink");
    m_labelTitle = new QLabel("Title");
    m_labelAuthor = new QLabel("Author");
    m_labelKeyword = new QLabel("Keywords");
    m_labelLayout = new QLabel("Layout");
    m_labelMenu = new QLabel("Menu");
    m_author = new QLineEdit;
    m_keywords = new QLineEdit;
    m_menus = new QComboBox;
    m_layouts = new QComboBox;
    m_layouts->setMaximumWidth(100);

    foreach(Menu *menu, m_site->menus())
    {
        m_menus->addItem(menu->name());
    }

    QDir layouts(m_site->sourcePath() + "/layouts");
    foreach(QString file, layouts.entryList(QDir::Files))
    {
        m_layouts->addItem(file.mid(0, file.indexOf(".html")));
    }

    QDir themelayouts(Generator::themesPath() + "/" + m_site->theme() + "/layouts");
    foreach(QString file, themelayouts.entryList(QDir::Files))
    {
        QString layout = file.mid(0, file.indexOf(".html"));
        if(m_layouts->findText(layout) < 0)
            m_layouts->addItem(layout);
    }

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
    m_layout->addLayout(hbox, 0, 3);
    m_layout->addWidget(m_labelTitle, 1, 0);
    m_layout->addWidget(m_title, 2, 0);
    m_layout->addWidget(m_labelPermalink, 1, 1);
    m_layout->addWidget(m_source, 2, 1);
    m_layout->addWidget(m_labelAuthor, 3, 0);
    m_layout->addWidget(m_author, 4, 0);
    m_layout->addWidget(m_labelKeyword, 3, 1);
    m_layout->addWidget(m_keywords, 4, 1);
    m_layout->addWidget(m_labelMenu, 3, 2);
    m_layout->addWidget(m_menus, 4, 2);
    m_layout->addWidget(m_labelLayout, 3, 3);
    m_layout->addWidget(m_layouts, 4, 3);
    m_layout->addWidget(m_scroll, 7, 0, 1, 4);
    m_vbox->addLayout(m_layout);
    setLayout(m_vbox);

    if(m_content->contentType() == ContentType::Post)
    {
        m_excerptLabel = new QLabel("Excerpt");
        m_layout->addWidget(m_excerptLabel, 5, 0);
        m_layout->addWidget(m_excerpt, 6, 0, 1, 2);

        m_datelabel = new QLabel("Date");
        m_layout->addWidget(m_datelabel, 5, 2, 0);
        m_layout->addWidget(m_date, 6, 2, 1, 2);
    }

    m_filename = m_site->sourcePath() + (content->contentType() == ContentType::Page ? "/pages/" : "/posts/") + content->source();

    load();

    connect(m_close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(m_redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(m_title, SIGNAL(editingFinished()), this, SLOT(titleChanged()));
    connect(m_title, SIGNAL(textChanged(QString)), this, SLOT(titleChanged(QString)));
    connect(m_source, SIGNAL(editingFinished()), this, SLOT(sourceChanged()));
    connect(m_excerpt, SIGNAL(editingFinished()), this, SLOT(excerptChanged()));
    connect(m_date, SIGNAL(editingFinished()), this, SLOT(dateChanged()));
    connect(m_author, SIGNAL(editingFinished()), this, SLOT(authorChanged()));
    connect(m_keywords, SIGNAL(editingFinished()), this, SLOT(keywordsChanged()));
    connect(m_menus, SIGNAL(currentIndexChanged(QString)), this, SLOT(menuChanged(QString)));
    connect(m_layouts, SIGNAL(currentIndexChanged(QString)), this, SLOT(layoutChanged(QString)));
    connect(m_previewLink, SIGNAL(clicked()), this, SLOT(preview()));
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

void ContentEditor::closeEditor()
{
    emit close();
}

void ContentEditor::siteLoaded(Site *site)
{
    m_site = site;
    if(m_content->contentType() == ContentType::Page)
    {
        foreach(Content *c, m_site->pages())
        {
            if(c->source() == m_content->source())
                m_title->setText(c->title());
        }
    }
    else
    {
        foreach(Content *c, m_site->posts())
        {
            if(c->source() == m_content->source())
            {
                m_excerpt->setText(c->excerpt());
                m_title->setText(c->title());
            }
        }
    }
}

void ContentEditor::titleChanged(QString title)
{
    if(m_isNew)
    {
        QString source = title.toLower().replace(" ", "_") + ".xml";
        m_source->setText(source);
    }
}

void ContentEditor::titleChanged()
{
    if(m_title->text() != m_content->title())
    {
        if(m_isNew)
            sourceChanged();

        m_content->setTitle(m_title->text());
        emit contentChanged(m_content);
        editChanged("Titel Changed");
    }
}

void ContentEditor::sourceChanged()
{
    if(m_source->text() != m_content->source())
    {
        QString oldname = m_filename;
        m_content->setSource(m_source->text());
        if(m_content->contentType() == ContentType::Page)
            m_filename = m_site->sourcePath() + "/pages/" + m_content->source();
        else
            m_filename = m_site->sourcePath() + "/posts/" + m_content->source();

        emit contentChanged(m_content);

        QUndoCommand *renameCommand = new RenameContentCommand(this, oldname, m_filename, "content file renamed");
        m_undoStack->push(renameCommand);
    }
}

void ContentEditor::contentRenamed(QString name)
{
    m_filename = name;
    QFileInfo info(name);
    m_source->setText(info.fileName());
    m_content->setSource(info.fileName());
}

void ContentEditor::excerptChanged()
{
    if(m_excerpt->text() != m_content->excerpt())
    {
        m_content->setExcerpt(m_excerpt->text());
        emit contentChanged(m_content);
        editChanged("Excerpt Changed");
    }
}

void ContentEditor::dateChanged()
{
    if(m_date->text() != m_content->date().toString("dd.MM.yyyy"))
    {
        m_content->setDate(QDate::fromString(m_date->text(), "dd.MM.yyyy"));
        emit contentChanged(m_content);
        editChanged("Date Changed");
    }
}

void ContentEditor::authorChanged()
{
    if(m_author->text() != m_content->author())
    {
        m_content->setAuthor(m_author->text());
        emit contentChanged(m_content);
        editChanged("Author Changed");
    }
}

void ContentEditor::keywordsChanged()
{
    if(m_keywords->text() != m_content->keywords())
    {
        m_content->setKeywords(m_keywords->text());
        emit contentChanged(m_content);
        editChanged("Keywords Changed");
    }
}

void ContentEditor::menuChanged(QString menu)
{
    if(menu != m_content->menu())
    {
        m_content->setMenu(menu);
        emit contentChanged(m_content);
        editChanged("Menu Changed");
    }
}

void ContentEditor::layoutChanged(QString layout)
{
    if(layout != m_content->layout())
    {
        m_content->setLayout(layout);
        emit contentChanged(m_content);
        editChanged("Layout Changed");
    }
}

void ContentEditor::load()
{
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        m_win->statusBar()->showMessage("ContentEditor::load(): Unable to open " + m_filename);
        return;
    }
    PageEditor *pe = new PageEditor();
    m_scroll->setWidget(pe);

    QXmlStreamReader stream(&file);
    if(stream.readNextStartElement())
    {
        if(stream.name() == "Content")
        {
            m_site->loadContent(m_content, &stream);
            if(m_content->contentType() == ContentType::Post)
            {
                m_excerpt->setText(m_content->excerpt());
                m_date->setText(m_content->date().toString("dd.MM.yyyy"));
            }

            if(!m_content->title().isEmpty())
                m_titleLabel->setText(m_content->contentType() == ContentType::Page ? "Edit Page" : "Edit Post");
            else
                m_titleLabel->setText(m_content->contentType() == ContentType::Page ? "Add new Page" : "Add new Post");

            m_isNew = m_content->title().isEmpty();
            m_title->setText(m_content->title());
            if(m_isNew)
                m_source->setText("");
            else
                m_source->setText(m_content->source());
            m_author->setText(m_content->author());
            m_keywords->setText(m_content->keywords());
            m_menus->setCurrentText(m_content->menu());
            m_layouts->setCurrentText(m_content->layout());

            while(stream.readNext())
            {
                if(stream.isStartElement())
                {
                    if(stream.name() == "Section")
                    {
                        bool fullwidth = stream.attributes().value("fullwidth").toString()  == "true";
                        SectionEditor *se = new SectionEditor(fullwidth);
                        se->setCssClass(stream.attributes().value("cssclass").toString());
                        se->setStyle(stream.attributes().value("style").toString());
                        se->setAttributes(stream.attributes().value("attributes").toString());
                        se->setId(stream.attributes().value("id").toString());
                        pe->addSection(se);
                        if(fullwidth)
                        {
                            while(stream.readNext())
                            {
                                if(stream.isStartElement())
                                {
                                    ElementEditor *ee = new ElementEditor();
                                    ee->setMode(ElementEditor::Mode::Enabled);
                                    ee->load(&stream);
                                    se->addElement(ee);
                                }
                                else if(stream.isEndElement())
                                    break;
                                else if(stream.atEnd())
                                {
                                    if(stream.hasError())
                                        m_win->statusBar()->showMessage(stream.errorString() + " in line " + QString::number(stream.lineNumber()) + " column " + QString::number(stream.columnNumber()));
                                    file.close();
                                    return;
                                }
                            }
                        }
                        else
                            loadRows(&stream, se);
                    }
                    else if(stream.name().isEmpty())
                        ; // ignore
                    else
                        m_win->statusBar()->showMessage("Wrong starttag " + stream.name() + " in line " + QString::number(stream.lineNumber()));
                }
                else if(stream.isEndElement() || stream.atEnd())
                    break;
            }
        }
    }
    file.close();
}

void ContentEditor::loadRows(QXmlStreamReader *stream, SectionEditor *se)
{
    while(stream->readNext())
    {
        if(stream->isStartElement())
        {
            if(stream->name() == "Row")
            {
                RowEditor *re = new RowEditor();
                re->setCssClass(stream->attributes().value("cssclass").toString());
                se->addRow(re);
                loadColumns(stream, re);
            }
            else if(stream->name().isEmpty())
                ; // ignore
            else
            {
                QString msg = "Wrong starttag " + stream->name() + " in line " + QString::number(stream->lineNumber());
                qWarning() << msg;
                m_win->statusBar()->showMessage(msg);
            }
        }
        else if(stream->isEndElement() || stream->atEnd())
            break;
    }
}

void ContentEditor::loadColumns(QXmlStreamReader *stream, RowEditor *re)
{
    int i = 0;
    while(stream->readNext())
    {
        if(stream->isStartElement())
        {
            if(stream->name() == "Column")
            {
                ColumnEditor *ce = new ColumnEditor();
                ce->setSpan(stream->attributes().value("span").toInt());
                re->addColumn(ce, i++);
                loadElements(stream, ce);
            }
            else if(stream->name().isEmpty())
                ; // ignore
            else
            {
                QString msg = "Wrong starttag " + stream->name() + " in line " + QString::number(stream->lineNumber());
                qWarning() << msg;
                m_win->statusBar()->showMessage(msg);
            }
        }
        else if(stream->isEndElement() || stream->atEnd())
            break;
    }
}

void ContentEditor::loadElements(QXmlStreamReader *stream, ColumnEditor *ce)
{
    QString tag  = "";
    while(stream->readNext())
    {
        if(stream->isStartElement())
        {
            tag = stream->name().toString();
            ElementEditor *ee = new ElementEditor();
            ee->setMode(ElementEditor::Mode::Enabled);
            ee->load(stream);
            ce->addElement(ee);
            if(stream->hasError())
            {
                QString msg = stream->errorString() + " in line " + QString::number(stream->lineNumber()) + " column " + QString::number(stream->columnNumber());
                qWarning() << msg;
                m_win->statusBar()->showMessage(msg);
                return;
            }
        }
        else if(stream->isEndElement())
        {
            if(stream->name() != tag)
                break;
        }
        else if(stream->atEnd())
            break;
    }
}


void ContentEditor::save()
{
    QFile file(m_filename);
    if(!file.open(QFile::WriteOnly))
    {
        QString msg = "ContentEditor::save(): Unable to open file " + m_filename;
        qWarning() << msg;
        m_win->statusBar()->showMessage(msg);
        return;
    }

    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Content");
    xml.writeAttribute("title", m_content->title());
    xml.writeAttribute("menu", m_content->menu());
    xml.writeAttribute("author", m_content->author());
    xml.writeAttribute("layout", m_content->layout());
    xml.writeAttribute("keywords", m_content->keywords());
    if(m_content->contentType() == ContentType::Post)
        xml.writeAttribute("excerpt", m_content->excerpt());
    xml.writeAttribute("date", QString(m_content->date().toString("dd.MM.yyyy")));

    foreach(QString attName, m_content->additionalAttributes().keys())
    {
        xml.writeAttribute(attName, m_content->additionalAttributes().value(attName));
    }

    PageEditor *pe = dynamic_cast<PageEditor*>(m_scroll->widget());
    foreach(SectionEditor *se, pe->sections())
    {
        se->save(&xml);
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void ContentEditor::editChanged(QString text)
{
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

    m_editor = new SectionPropertyEditor();
    m_editor->setContent(se->content());
    connect(m_editor, SIGNAL(close()), this, SLOT(sectionEditorClose()));
    animate(se);
}

void ContentEditor::rowEdit(RowEditor *re)
{
    m_rowEditor = re;

    m_editor = new RowPropertyEditor();
    m_editor->setContent(re->content());
    connect(m_editor, SIGNAL(close()), this, SLOT(rowEditorClose()));

    animate(re);
}

void ContentEditor::elementEdit(ElementEditor *ee)
{
    m_elementEditor = ee;
    if(Plugins::hasElementPlugin(ee->type()))
        m_editor = dynamic_cast<AnimateableEditor*>(Plugins::getElementPlugin(ee->type()));
    else
    {
        m_editor = dynamic_cast<AnimateableEditor*>(Plugins::getElementPlugin("TextEditor"));
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
    m_author->setEnabled(false);
    m_keywords->setEnabled(false);
    m_menus->setEnabled(false);
    m_layouts->setEnabled(false);
    m_labelAuthor->setEnabled(false);
    m_labelKeyword->setEnabled(false);
    m_labelMenu->setEnabled(false);
    m_labelLayout->setEnabled(false);
    m_labelTitle->setEnabled(false);
    m_labelPermalink->setEnabled(false);
    m_previewLink->hide();
    m_undo->hide();
    m_redo->hide();
    m_close->hide();
    m_source->setEnabled(false);
    if(m_content->contentType() == ContentType::Post)
    {
        m_excerpt->setEnabled(false);
        m_excerptLabel->setEnabled(false);
    }
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
    m_author->setEnabled(true);
    m_keywords->setEnabled(true);
    m_menus->setEnabled(true);
    m_layouts->setEnabled(true);
    m_labelAuthor->setEnabled(true);
    m_labelKeyword->setEnabled(true);
    m_labelMenu->setEnabled(true);
    m_labelLayout->setEnabled(true);
    m_labelTitle->setEnabled(true);
    m_labelPermalink->setEnabled(true);
    m_previewLink->show();
    m_undo->show();
    m_redo->show();
    m_close->show();
    if(m_content->contentType() == ContentType::Post)
    {
        m_excerpt->setEnabled(true);
        m_excerptLabel->setEnabled(true);
    }
    delete m_animationgroup;
    m_editor->hide();
    // parent has to be set to NULL, otherwise the plugin will be dropped by parent
    m_editor->setParent(NULL);
    disconnect(m_editor, SIGNAL(close()), this, SLOT(editorClose()));
    m_editor = NULL;
}
