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

#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include "interfaces.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>

class TextEditor : public EditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.crowdware.FlatSiteBuilder.EditorInterface" FILE "texteditor.json")
    Q_INTERFACES(EditorInterface)

public:
    TextEditor();

    void setContent(QDomElement ele);
    QString className() {return "TextEditor";}
    QString displayName() {return "Text";}
    QString tagName() {return "Text";}
    QImage icon() {return QImage(":/images/text.png");}
    QString getHtml(QDomElement ele, QMap<QString, EditorInterface*> plugins);

signals:
    void close();

private slots:
    void closeEditor();

private:
    QTextEdit *m_html;
    QLineEdit *m_adminlabel;
};

#endif // TEXTEDITOR_H
