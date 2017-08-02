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
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include "content.h"
#include "site.h"
#include "elementeditor.h"
#include "texteditor.h"

class ContentEditor : public QWidget
{
    Q_OBJECT

public:
    ContentEditor(Site *site, Content *content = NULL);

    void elementEdit(ElementEditor *);

private slots:
    void save();
    void editChanged();
    void preview();
    void editorClose(QWidget *);
    void animationFineshed();

signals:
    void contentUpdated();
    void preview(Content *);

private:
    Site *m_site;
    Content *m_content;
    QPushButton *m_save;
    QLabel *m_titleLabel;
    QTextEdit *m_text;
    QString m_filename;
    QLineEdit *m_title;
    QLineEdit *m_excerpt;
    QScrollArea *m_scroll;
    QGridLayout *m_layout;
    QVBoxLayout *m_vbox;
    QLabel *m_excerptLabel;
    Hyperlink *m_previewLink;
    QWidget *m_animationPanel;
    TextEditor *m_editor;
};

#endif // CONTENTEDITOR_H
