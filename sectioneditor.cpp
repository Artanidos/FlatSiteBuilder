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
#include <QDrag>

SectionEditor::SectionEditor(bool fullwidth)
{
    m_fullwidth = fullwidth;
    QPalette pal = palette();
    if(m_fullwidth)
        pal.setColor(QPalette::Background, QColor("#800080"));
    else
        pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);
    setAcceptDrops(true);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setAlignment(Qt::AlignTop);
    vbox->setSpacing(5);
    m_editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    m_closeButton = new FlatButton(":/images/close_normal.png", ":/images/close_hover_red.png");
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
    vboxRight->addWidget(addRow);
    layout->addLayout(vboxRight);
    setLayout(layout);

    connect(addRow, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(m_editButton, SIGNAL(clicked()), this, SLOT(edit()));
}

void SectionEditor::save(QDomDocument doc, QDomElement de)
{
    QDomElement section = doc.createElement("Section");
    if(!m_id.isEmpty())
        section.setAttribute("id", m_id);
    if(!m_cssclass.isEmpty())
        section.setAttribute("cssclass", m_cssclass);
    if(!m_style.isEmpty())
        section.setAttribute("style", m_style);
    if(!m_attributes.isEmpty())
        section.setAttribute("attributes", m_attributes);
    if(m_fullwidth)
        section.setAttribute("fullwidth", "true");
    de.appendChild(section);
    for(int i = 0; i < m_layout->count(); i++)
    {
        RowEditor *re = dynamic_cast<RowEditor*>(m_layout->itemAt(i)->widget());
        if(re)
            re->save(doc, section);
    }
}

void SectionEditor::addRow(RowEditor *re)
{
    connect(re, SIGNAL(rowEditorCopied(RowEditor*)), this, SLOT(copyRowEditor(RowEditor *)));
    m_layout->addWidget(re);
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
        if(re)
        {
            // insert a dropzone at the end
            m_layout->addWidget(new DropZone(myData->width(), myData->height()));
            event->accept();
        }
        else
            event->ignore();
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
            event->ignore();
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
            event->ignore();
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

void SectionEditor::setContent(QDomElement section)
{
    m_cssclass = section.attribute("cssclass");
    m_style = section.attribute("style");
    m_attributes = section.attribute("attributes");
    m_id = section.attribute("id");
}

QDomElement SectionEditor::content()
{
    QDomElement section = m_doc.createElement("Section");
    section.setAttribute("cssclass", m_cssclass);
    section.setAttribute("style", m_style);
    section.setAttribute("attributes", m_attributes);
    section.setAttribute("id", m_id);
    return section;
}
