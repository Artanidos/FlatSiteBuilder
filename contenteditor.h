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
#include <QPropertyAnimation>
#include "content.h"
#include "interfaces.h"
#include "site.h"
#include "sectioneditor.h"
#include "rowpropertyeditor.h"
#include "sectionpropertyeditor.h"
#include "interfaces.h"

class QStatusBar;
class MainWindow;
class ContentEditor : public AbstractEditor
{
    Q_OBJECT

public:
    ContentEditor(MainWindow *win, Site *site, Content *content = NULL);
    ~ContentEditor();

    bool eventFilter(QObject *watched, QEvent *event);
    void elementEdit(ElementEditor *);
    void rowEdit(RowEditor *);
    void sectionEdit(SectionEditor *);
    Content *getContent() {return m_content;}
    Site *site() {return m_site;}
    QString filename() {return m_filename;}
    void load();
    void setStatusBar(QStatusBar *bar) {m_statusbar = bar;}\
    void setContent(QDomElement) {/* unused */}

public slots:
    void editChanged(QString text);
    void save();
    void closeEditor();

private slots:
    void preview();
    void editorClose();
    void rowEditorClose();
    void sectionEditorClose();
    void animationFineshedZoomIn();
    void animationFineshedZoomOut();
    void titleChanged();
    void titleChanged(QString title);
    void sourceChanged();
    void excerptChanged();
    void siteLoaded(Site *);
    void showContextMenu(const QPoint &);
    void canUndoChanged(bool can);
    void canRedoChanged(bool can);
    void undoTextChanged(QString text);
    void redoTextChanged(QString text);
    void undo();
    void redo();

signals:
    void contentUpdated(QString text);
    void preview(Content *);
    void contentEditorClosed();
    void contentChanged(Content *content);
    void close();

private:
    Site *m_site;
    Content *m_content;
    QLabel *m_titleLabel;
    QLabel *m_labelTitle;
    QLabel *m_labelPermalink;
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
    QPropertyAnimation *m_animx;
    QPropertyAnimation *m_animy;
    QPropertyAnimation *m_animw;
    QPropertyAnimation *m_animh;
    ElementEditor *m_elementEditor;
    RowEditor *m_rowEditor;
    SectionEditor *m_sectionEditor;
    QUndoStack *m_undoStack;
    FlatButton *m_close;
    FlatButton *m_undo;
    FlatButton *m_redo;
    QWidget *m_sourcewidget;
    QStatusBar *m_statusbar;
    MainWindow *m_win;
    bool m_isNew;

    void init();
    void loadRows(QDomElement section, SectionEditor *se);
    void loadColumns(QDomElement row, RowEditor *re);
    void loadElements(QDomElement column, ColumnEditor *ce);
    void animate(QWidget *widget);
    void editorClosed();
    void updateNewContent();
};

#endif // CONTENTEDITOR_H
