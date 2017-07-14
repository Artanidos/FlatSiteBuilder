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
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include "content.h"
#include "site.h"

class ContentEditor : public QWidget
{
    Q_OBJECT

public:
    ContentEditor(Site *site, Content *content = NULL);

private slots:
    void save();
    void editChanged();

signals:
    void contentUpdated();

private:
    Site *m_site;
    Content *m_content;
    QPushButton *m_save;
    QLabel *m_titleLabel;
    QTextEdit *m_text;
    QString m_filename;
    QLineEdit *m_title;
    QLineEdit *m_excerpt;
};

#endif // CONTENTEDITOR_H
