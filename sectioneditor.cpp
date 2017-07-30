#include "sectioneditor.h"
#include "hyperlink.h"
#include "widgetmimedata.h"
#include "dropzone.h"
#include <QTest>

SectionEditor::SectionEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);
    setAcceptDrops(true);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setAlignment(Qt::AlignTop);
    vbox->setSpacing(5);
    m_editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    m_closeButton = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
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

    connect(addRow, SIGNAL(linkActivated(QString)), this, SLOT(addRow()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void SectionEditor::addRow(RowEditor *re)
{
    m_layout->addWidget(re);
}

void SectionEditor::removeRow(RowEditor *re)
{
    m_layout->removeWidget(re);
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
}

void SectionEditor::close()
{
    delete this;
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
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
            event->ignore();
    }
    else
        event->ignore();
}

