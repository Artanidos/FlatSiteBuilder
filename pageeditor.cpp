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

#include "pageeditor.h"
#include "dropzone.h"
#include "widgetmimedata.h"
#include "contenteditor.h"
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>

PageEditor::PageEditor()
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setAlignment(Qt::AlignLeft);
    hbox->setSpacing(10);
    Hyperlink *addSection = new Hyperlink("(+) Add Section");
    Hyperlink *addFullSection = new Hyperlink("(+) Add Full Width Section");
    m_layout = new QVBoxLayout();
    hbox->addWidget(addSection);
    hbox->addWidget(addFullSection);
    m_layout->addLayout(hbox);
    m_layout->addStretch();
    setLayout(m_layout);
    setAcceptDrops(true);
    connect(addSection, SIGNAL(clicked()), this, SLOT(addSection()));
    connect(addFullSection, SIGNAL(clicked()), this, SLOT(addFullSection()));
}

void PageEditor::addSection(SectionEditor *se)
{
    connect(se, SIGNAL(sectionEditorCopied(SectionEditor*)), this, SLOT(copySection(SectionEditor*)));
    m_layout->insertWidget(m_layout->count() - 2, se);
}

QList<SectionEditor*> PageEditor::sections()
{
    QList<SectionEditor*> list;
    for(int i = 0; i < m_layout->count(); i++)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(m_layout->itemAt(i)->widget());
        if(se)
            list.append(se);
    }
    return list;
}

void PageEditor::addSection()
{
    addSection(new SectionEditor());
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged();
}

void PageEditor::addFullSection()
{
    addSection(new SectionEditor(true));
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged();
}

void PageEditor::copySection(SectionEditor *se)
{
    addSection(se->clone());
    ContentEditor *ce = getContentEditor();
    if(ce)
        ce->editChanged();
}

void PageEditor::removeSection(SectionEditor *se)
{
    m_layout->removeWidget(se);
}

void PageEditor::enableColumnAcceptDrop(bool mode)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(m_layout->itemAt(i)->widget());
        if(se)
            se->enableColumnAcceptDrop(mode);
    }
}

void PageEditor::enableSectionAcceptDrop(bool mode)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(m_layout->itemAt(i)->widget());
        if(se)
            se->setAcceptDrops(mode);
    }
}

void PageEditor::dragEnterEvent(QDragEnterEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if(myData)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(myData->getData());
        if(se)
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

void PageEditor::dragLeaveEvent(QDragLeaveEvent *event)
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

void PageEditor::dragMoveEvent(QDragMoveEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if(myData)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(myData->getData());
        if(se)
        {
            int height = 0;
            int row = 0;

            // evaluate position for the dropzone to be placed
            for(int i = 0; i < m_layout->count(); i++)
            {
                SectionEditor *editor = dynamic_cast<SectionEditor*>(m_layout->itemAt(i)->widget());
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

void PageEditor::dropEvent(QDropEvent *event)
{
    const WidgetMimeData *myData = qobject_cast<const WidgetMimeData *>(event->mimeData());
    if (myData)
    {
        SectionEditor *se = dynamic_cast<SectionEditor*>(myData->getData());
        if(se)
        {
            // place the dragged SectionEditor to the place where DropZone is now
            for(int i = 0; i < m_layout->count(); i++)
            {
                DropZone *dz = dynamic_cast<DropZone*>(m_layout->itemAt(i)->widget());
                if(dz)
                {
                    m_layout->replaceWidget(dz, se);
                    se->show();
                    delete dz;
                    break;
                }
            }
            ContentEditor *ce = getContentEditor();
            if(ce)
                ce->editChanged();
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
            event->ignore();
    }
    else
        event->ignore();
}

ContentEditor* PageEditor::getContentEditor()
{

    QWidget *sa = dynamic_cast<QWidget*>(parentWidget());
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
    return NULL;
}
