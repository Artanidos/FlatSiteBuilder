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

#ifndef ELEMENTEDITOR_H
#define ELEMENTEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "columneditor.h"
#include "hyperlink.h"
#include "flatbutton.h"

class ContentEditor;
class QXmlStreamWriter;
class QXmlStreamReader;
class ElementEditor : public QWidget
{
    Q_OBJECT

public:
    ElementEditor();

    void mousePressEvent(QMouseEvent *event) override;

    ElementEditor *clone();
    void setContent(QString content);
    QString content() {return m_content;}
    enum Mode {Empty, Enabled, Dropzone};
    void setMode(Mode mode);
    Mode mode() {return m_mode;}
    QString type() {return m_type;}
    void save(QXmlStreamWriter *);
    void load(QXmlStreamReader *);

public slots:
    void dropped();

private slots:
    void enable();
    void close();
    void edit();
    void copy();

signals:
    void elementEnabled();
    void elementDragged();
    void elementCopied(ElementEditor*);

private:
    Hyperlink *m_link;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
    QLabel *m_text;
    QString m_normalColor;
    QString m_enabledColor;
    QString m_dropColor;
    QString m_content;
    Mode m_mode;
    QString m_type;
    bool m_zoom;

    void setColor(QString name);
    ContentEditor* getContentEditor();
    void setText(QString text) {m_text->setText(text);}
};

#endif // ELEMENTEDITOR_H
