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

#include "columneditor.h"
#include "elementeditor.h"
#include "widgetmimedata.h"
#include "contenteditor.h"
#include "pageeditor.h"
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QTest>

ColumnEditor::ColumnEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()).lighter());
    setPalette(pal);
    setAutoFillBackground(true);

    m_layout = new QVBoxLayout();
    m_layout->setAlignment(Qt::AlignTop);
    setLayout(m_layout);
    setAcceptDrops(true);

    ElementEditor *ee = new ElementEditor();
    m_layout->addWidget(ee, 0, Qt::AlignTop);

    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
}

void ColumnEditor::save(QDomDocument doc, QDomElement de)
{
    QDomElement col = doc.createElement("Column");
    for(int i = 0; i < m_layout->count(); i++)
    {
        ElementEditor *ee = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
        if(ee)
            ee->save(doc, col);
    }
    de.appendChild(col);
}

void ColumnEditor::addElement()
{
    ElementEditor *ee = new ElementEditor();
    m_layout->addWidget(ee, 0, Qt::AlignTop);

    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged();
    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
}

void ColumnEditor::copyElement(ElementEditor *e)
{
    addElement(e->clone());
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged();
}

void ColumnEditor::addElement(ElementEditor *ee)
{
    m_layout->insertWidget(m_layout->count() - 1, ee, 0, Qt::AlignTop);
    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
}

ColumnEditor* ColumnEditor::clone()
{
    ColumnEditor *nce = new ColumnEditor();
    for(int i = 0; i < m_layout->count(); i++)
    {
        ElementEditor *ee = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
        if(ee->mode() != ElementEditor::Mode::Empty)
            nce->addElement(ee->clone());
    }
    return nce;
}

void ColumnEditor::dragEnterEvent(QDragEnterEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if(myData)
    {
        ElementEditor *ee = dynamic_cast<ElementEditor*>(myData->getData());
        if(ee)
        {
            for(int i = 0; i < m_layout->count(); i++)
            {
                ElementEditor *editor = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
                if(editor && editor->mode() == ElementEditor::Mode::Empty)
                {
                    editor->setMode(ElementEditor::Mode::Dropzone);
                    break;
                }
            }
            event->accept();
        }
        else
            event->ignore();
    }
    else
        event->ignore();
}

void ColumnEditor::dragLeaveEvent(QDragLeaveEvent *event)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        ElementEditor *editor = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
        if(editor && editor->mode() == ElementEditor::Mode::Dropzone)
        {
            // put editor to the end of the list
            editor->setMode(ElementEditor::Mode::Empty);
            m_layout->removeWidget(editor);
            m_layout->addWidget(editor);
            break;
        }
    }
    event->accept();
}

void ColumnEditor::dragMoveEvent(QDragMoveEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if(myData)
    {
        ElementEditor *ee = dynamic_cast<ElementEditor*>(myData->getData());
        if(ee)
        {
            int row = event->pos().y() / (50 + m_layout->margin());
            for(int i = 0; i < m_layout->count(); i++)
            {
                ElementEditor *editor = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
                if(editor && editor->mode() == ElementEditor::Mode::Dropzone)
                {
                    if(i != row)
                    {
                        // put dropzone under mouse pointer
                        m_layout->insertWidget(row, editor);
                    }
                    break;
                }
            }
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
            event->ignore();
    }
    else
        event->ignore();
}

void ColumnEditor::dropEvent(QDropEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if (myData)
    {
        ElementEditor *ee = dynamic_cast<ElementEditor*>(myData->getData());
        if(ee)
        {
            for(int i = 0; i < m_layout->count(); i++)
            {
                ElementEditor *editor = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
                if(editor && editor->mode() == ElementEditor::Mode::Dropzone)
                {
                    // put editor to the end of the list
                    editor->setMode(ElementEditor::Mode::Empty);
                    m_layout->removeWidget(editor);
                    m_layout->addWidget(editor);
                    break;
                }
            }

            m_layout->insertWidget(0, ee, 0, Qt::AlignTop);
            ee->dropped();
            ee->show();
            ee->disconnect(SIGNAL(elementEnabled()));
            ee->disconnect(SIGNAL(elementDragged()));
            ee->disconnect(SIGNAL(elementCopied(ElementEditor*)));
            ContentEditor *ce = getContentEditor();
            if(ce)
                ce->editChanged();
            connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
            connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
            connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
            event->ignore();
    }
    else
        event->ignore();
}

ContentEditor* ColumnEditor::getContentEditor()
{
    RowEditor *re = dynamic_cast<RowEditor*>(parentWidget());
    if(re)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(re->parentWidget());
        if(se)
        {
            PageEditor *pe = dynamic_cast<PageEditor*>(se->parentWidget());
            if(pe)
            {
                QWidget *sa = dynamic_cast<QWidget*>(pe->parentWidget());
                if(sa)
                {
                    QWidget *vp = dynamic_cast<QWidget*>(sa->parentWidget());
                    if(vp)
                    {
                        ContentEditor *cee = dynamic_cast<ContentEditor*>(vp->parentWidget());
                        if(cee)
                            return cee;
                    }
                }
            }
        }
    }
    return NULL;
}
