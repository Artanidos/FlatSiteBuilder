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

#include "sectioneditor.h"
#include "hyperlink.h"
#include "widgetmimedata.h"
#include "dropzone.h"
#include "pageeditor.h"
#include "contenteditor.h"
#include <QTest>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDrag>

SectionEditor::SectionEditor(bool fullwidth)
{
    m_fullwidth = fullwidth;
    setAutoFillBackground(true);
    setAcceptDrops(true);
    setBGColor();
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setAlignment(Qt::AlignTop);
    vbox->setSpacing(5);
    m_editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    m_closeButton = new FlatButton(":/images/trash_normal.png", ":/images/trash_hover.png");
    m_editButton->setToolTip("Edit Section");
    m_closeButton->setToolTip("Delete Section");
    m_copyButton->setToolTip("Copy Section");
    vbox->addWidget(m_editButton);
    vbox->addWidget(m_copyButton);
    vbox->addWidget(m_closeButton);

    QVBoxLayout *vboxRight = new QVBoxLayout();
    vboxRight->setAlignment(Qt::AlignLeft);
    QHBoxLayout *layout = new QHBoxLayout();
    m_layout = new QVBoxLayout();
    layout->addLayout(vbox);
    Hyperlink *addRow = new Hyperlink("(+) Add Row");
    vboxRight->addLayout(m_layout);

    ElementEditor *ee = new ElementEditor();
    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));

    if(m_fullwidth)
        m_layout->addWidget(ee, 0, Qt::AlignTop);
    else
        vboxRight->addWidget(addRow);
    layout->addLayout(vboxRight);
    setLayout(layout);

    connect(addRow, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(m_editButton, SIGNAL(clicked()), this, SLOT(edit()));
}

void SectionEditor::setBGColor()
{
    QPalette pal = palette();
    if(m_fullwidth)
        pal.setColor(QPalette::Background, QColor("#800080"));
    else
        pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
}

void SectionEditor::save(QXmlStreamWriter *stream)
{
    stream->writeStartElement("Section");
    if(!m_id.isEmpty())
        stream->writeAttribute("id", m_id);
    if(!m_cssclass.isEmpty())
        stream->writeAttribute("cssclass", m_cssclass);
    if(!m_style.isEmpty())
        stream->writeAttribute("style", m_style);
    if(!m_attributes.isEmpty())
        stream->writeAttribute("attributes", m_attributes);
    if(m_fullwidth)
        stream->writeAttribute("fullwidth", "true");
    stream->writeCharacters("");
    for(int i = 0; i < m_layout->count(); i++)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(m_layout->itemAt(i)->widget());
        if(re)
            re->save(stream);
        ElementEditor *ee = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
        if(ee)
            ee->save(stream);
    }
    stream->writeEndElement();
}

void SectionEditor::addRow(RowEditor *re)
{
    connect(re, SIGNAL(rowEditorCopied(RowEditor*)), this, SLOT(copyRowEditor(RowEditor *)));
    m_layout->addWidget(re);
}

void SectionEditor::addElement(ElementEditor *ee)
{
    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
    m_layout->insertWidget(m_layout->count() - 1, ee, 0, Qt::AlignTop);
}

void SectionEditor::removeRow(RowEditor *re)
{
    m_layout->removeWidget(re);
}

void SectionEditor::copyRowEditor(RowEditor *re)
{
    addRow(re->clone());
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged("Copy Row");
}

void SectionEditor::enableColumnAcceptDrop(bool mode)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(m_layout->itemAt(i)->widget());
        if(re)
            re->enableColumnAcceptDrop(mode);
    }
}

void SectionEditor::addRow()
{
    addRow(new RowEditor());
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged("Add Row");
}

void SectionEditor::close()
{
    PageEditor *pe = dynamic_cast<PageEditor*>(parentWidget());
    if(pe)
        pe->removeSection(this);
    this->deleteLater();
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged("Delete Section");
}

void SectionEditor::copy()
{
    emit sectionEditorCopied(this);
}

void SectionEditor::edit()
{
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->sectionEdit(this);
}

SectionEditor *SectionEditor::clone()
{
    SectionEditor *ne = new SectionEditor(m_fullwidth);
    ne->setCssClass(m_cssclass);
    ne->setId(m_id);
    ne->setAttributes(m_attributes);
    ne->setStyle(m_style);
    for(int i = 0; i < m_layout->count(); i++)
    {
        ne->addRow(dynamic_cast<RowEditor*>(m_layout->itemAt(i)->widget())->clone());
    }
    return ne;
}

void SectionEditor::dragEnterEvent(QDragEnterEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if(myData)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(myData->getData());
        if(!m_fullwidth && re)
        {
            // insert a dropzone at the end
            m_layout->addWidget(new DropZone(myData->width(), myData->height()));
            event->accept();
        }
        else
        {
            ElementEditor *ee = dynamic_cast<ElementEditor*>(myData->getData());
            if(m_fullwidth && ee)
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
    }
    else
        event->ignore();
}

void SectionEditor::dragLeaveEvent(QDragLeaveEvent *event)
{
    // remove dropzones
    for(int i = 0; i < m_layout->count(); i++)
    {
        DropZone *dz = dynamic_cast<DropZone*>(m_layout->itemAt(i)->widget());
        if(dz)
        {
            delete dz;
            break;
        }
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

void SectionEditor::dragMoveEvent(QDragMoveEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if(myData)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(myData->getData());
        if(re)
        {
            int height = 0;
            int row = 0;

            // evaluate position for the dropzone to be placed
            for(int i = 0; i < m_layout->count(); i++)
            {
                RowEditor *editor = dynamic_cast<RowEditor*>(m_layout->itemAt(i)->widget());
                if(editor)
                {

                    if(event->pos().y() > height && event->pos().y() < height + editor->height())
                    {
                        break;
                    }
                    height += editor->height();

                    row++;
                }
            }

            // find dropzone and replace it to new location
            for(int i = 0; i < m_layout->count(); i++)
            {
                DropZone *dz = dynamic_cast<DropZone*>(m_layout->itemAt(i)->widget());
                if(dz)
                {
                    if(i != row)
                    {
                        m_layout->insertWidget(row, dz);
                    }
                    break;
                }
            }
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
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

    }
    else
        event->ignore();
}

void SectionEditor::dropEvent(QDropEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if (myData)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(myData->getData());
        if(re)
        {
            // place the dragged RowEditor to the place where DropZone is now
            for(int i = 0; i < m_layout->count(); i++)
            {
                DropZone *dz = dynamic_cast<DropZone*>(m_layout->itemAt(i)->widget());
                if(dz)
                {
                    m_layout->replaceWidget(dz, re);
                    re->show();
                    delete dz;
                    break;
                }
            }
            ContentEditor *ce = getContentEditor();
            if(ce)
                ce->editChanged("Move Row");
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            ElementEditor *ee = dynamic_cast<ElementEditor*>(myData->getData());
            if(ee)
            {
                for(int i = 0; i < m_layout->count(); i++)
                {
                    ElementEditor *dz = dynamic_cast<ElementEditor*>(m_layout->itemAt(i)->widget());
                    if(dz && dz->mode() == ElementEditor::Mode::Dropzone)
                    {
                        // remove widget if it belongs to this layout
                        m_layout->removeWidget(ee);

                        // replace dropzone with dragged element
                        m_layout->replaceWidget(dz, ee);

                        // and put dropzone to the end of the list
                        dz->setMode(ElementEditor::Mode::Empty);
                        m_layout->removeWidget(dz);
                        m_layout->addWidget(dz);
                        break;
                    }
                }
                ee->dropped();
                ee->show();
                ee->disconnect(SIGNAL(elementEnabled()));
                ee->disconnect(SIGNAL(elementDragged()));
                ee->disconnect(SIGNAL(elementCopied(ElementEditor*)));
                connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
                connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
                connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));

                ContentEditor *ce = getContentEditor();
                if(ce)
                    ce->editChanged("Move Element");
                event->setDropAction(Qt::MoveAction);
                event->accept();
            }
            else
                event->ignore();
        }
    }
    else
        event->ignore();
}

void SectionEditor::mousePressEvent(QMouseEvent *event)
{
    WidgetMimeData *mimeData = new WidgetMimeData();
    mimeData->setSize(size().width(), size().height());
    mimeData->setData(this);

    QPixmap pixmap(this->size());
    render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos());
    drag->setPixmap(pixmap);

    PageEditor *pe = dynamic_cast<PageEditor*>(parentWidget());
    pe->removeSection(this);
    pe->enableColumnAcceptDrop(false);
    pe->enableSectionAcceptDrop(false);
    this->hide();

    if(drag->exec(Qt::MoveAction) == Qt::IgnoreAction)
    {
        pe->addSection(this);
        this->show();
    }
    pe->enableColumnAcceptDrop(true);
    pe->enableSectionAcceptDrop(true);
}

ContentEditor* SectionEditor::getContentEditor()
{

    PageEditor *pe = dynamic_cast<PageEditor*>(parentWidget());
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

    return NULL;
}

void SectionEditor::setContent(QString content)
{
    QXmlStreamReader stream(content);
    stream.readNextStartElement();
    m_cssclass = stream.attributes().value("cssclass").toString();
    m_style = stream.attributes().value("style").toString();
    m_attributes = stream.attributes().value("attributes").toString();
    m_id = stream.attributes().value("id").toString();
    m_fullwidth = stream.attributes().value("fullwidth").toString() == "true";
    setBGColor();
}

QString SectionEditor::content()
{
    QString content;
    QXmlStreamWriter stream(&content);
    stream.writeStartElement("Section");
    stream.writeAttribute("cssclass", m_cssclass);
    stream.writeAttribute("style", m_style);
    stream.writeAttribute("attributes", m_attributes);
    stream.writeAttribute("id", m_id);
    stream.writeAttribute("fullwidth", m_fullwidth ? "true" : "false");
    stream.writeEndElement();
    return content;
}

void SectionEditor::addElement()
{
    ElementEditor *ee = new ElementEditor();
    m_layout->addWidget(ee, 0, Qt::AlignTop);

    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged("Add Element");
    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
}

void SectionEditor::copyElement(ElementEditor *e)
{
    addElement(e->clone());
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged("Copy Element");
}
