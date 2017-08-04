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

#include <QWidget>
#include <QTextEdit>

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    TextEditor();

    void setContent(QString content) {m_html->setPlainText(content);}
    QString content() {return m_html->toPlainText();}
    bool changed() {return m_changed;}

signals:
    void close(QWidget*);

private slots:
    void save();
    void cancel();
    void textChanged();

private:
    QTextEdit *m_html;
    bool m_changed;
};

#endif // TEXTEDITOR_H
