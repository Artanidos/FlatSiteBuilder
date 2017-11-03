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

#include "rowpropertyeditor.h"
#include "column.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QXmlStreamWriter>
#include <QPushButton>
#include <QLabel>
#include <QTest>

RowPropertyEditor::RowPropertyEditor()
{
    m_changed = false;
    m_grid = new QGridLayout();
    m_grid->setMargin(0);
    m_cssclass = new QLineEdit();
    setAutoFillBackground(true);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    QLabel *titleLabel = new QLabel("Row Module Settings");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch();
    m_grid->addWidget(titleLabel, 0, 0);
    m_grid->addWidget(close, 0, 1, 1, 1, Qt::AlignRight);
    m_grid->addWidget(new QLabel("CSS Class"), 1, 0);
    m_grid->addWidget(m_cssclass, 2, 0, 1, 2);
    m_grid->addLayout(vbox, 3, 0);
    setLayout(m_grid);

    connect(close, SIGNAL(clicked()), this, SLOT(editorClose()));
    connect(m_cssclass, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
}

void RowPropertyEditor::editorClose()
{
    if(m_changed)
    {
        m_content = "";
        QXmlStreamWriter stream(&m_content);
        stream.writeStartElement("Row");
        stream.writeAttribute("cssclass", m_cssclass->text());
        stream.writeEndElement();
    }
    emit close();
}

void RowPropertyEditor::setContent(QString content)
{
    m_content = content;
    QXmlStreamReader stream(m_content);
    stream.readNextStartElement();
    m_cssclass->setText(stream.attributes().value("cssclass").toString());
    m_changed = false;
}

QString RowPropertyEditor::getHtml(QXmlStreamReader *xml, QString filename)
{
    QString cls = xml->attributes().value("cssclass").toString();
    QString html = "<div class=\"row" + (cls.isEmpty() ? "" : " " + cls) + "\">\n";
    while(xml->readNext())
    {
        if(xml->isStartElement())
        {
            if(xml->name() == "Column")
            {
                Column *c = new Column();
                html += c->getHtml(xml, filename);
            }
            else if(xml->name().isEmpty())
                ; // ignore
            else
                qWarning() << "Wrong starttag " + xml->name() + " in line " + QString::number(xml->lineNumber()) + " in file " + filename;
        }
        else if(xml->isEndElement() || xml->atEnd())
            break;
    }
    return html + "</div>\n";
}
