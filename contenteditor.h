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

#ifndef CONTENTEDITOR_H
#define CONTENTEDITOR_H

#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDomElement>
#include <QUndoStack>
#include <QParallelAnimationGroup>
#include "content.h"
#include "site.h"
#include "texteditor.h"
#include "sectioneditor.h"
#include "rowpropertyeditor.h"
#include "sectionpropertyeditor.h"

class ContentEditor : public QWidget
{
    Q_OBJECT

public:
    ContentEditor(Site *site, Content *content = NULL);

    void elementEdit(ElementEditor *);
    void rowEdit(RowEditor *);
    void sectionEdit(SectionEditor *);
    void setUndoStack(QUndoStack *stack);
    Content *content() {return m_content;}
    Site *site() {return m_site;}
    QString filename() {return m_filename;}
    void load();

public slots:
    void editChanged(QString text);
    void save();
    void close();

private slots:
    void preview();
    void editorClose(QWidget *);
    void rowEditorClose(QWidget *);
    void sectionEditorClose(QWidget *);
    void animationFineshedZoomIn();
    void animationFineshedZoomOut();
    void titleChanged();
    void titleChanged(QString title);
    void sourceChanged();
    void excerptChanged();
    void siteLoaded(Site *);

signals:
    void contentUpdated(QString text);
    void preview(Content *);
    void contentEditorClosed(QWidget *w);
    void contentHasChanged(Content *content);

private:
    Site *m_site;
    Content *m_content;
    QLabel *m_titleLabel;
    QTextEdit *m_text;
    QString m_filename;
    QLineEdit *m_title;
    QLineEdit *m_source;
    QLineEdit *m_excerpt;
    QScrollArea *m_scroll;
    QGridLayout *m_layout;
    QVBoxLayout *m_vbox;
    QLabel *m_excerptLabel;
    Hyperlink *m_previewLink;
    QWidget *m_animationPanel;
    AbstractEditor *m_editor;
    QParallelAnimationGroup *m_animationgroup;
    ElementEditor *m_elementEditor;
    RowEditor *m_rowEditor;
    SectionEditor *m_sectionEditor;
    QUndoStack *m_undoStack;
    FlatButton *m_close;
    bool m_isNew;

    void init();
    void loadRows(QDomElement section, SectionEditor *se);
    void loadColumns(QDomElement row, RowEditor *re);
    void loadElements(QDomElement column, ColumnEditor *ce);
    void animate(QWidget *widget);
    void editorClosed(QWidget *w);
    void updateNewContent();
};

#endif // CONTENTEDITOR_H
