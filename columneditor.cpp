#include "columneditor.h"
#include "elementeditor.h"
#include "widgetmimedata.h"
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

void ColumnEditor::addElement()
{
    ElementEditor *ee = new ElementEditor();
    m_layout->addWidget(ee, 0, Qt::AlignTop);

    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
}

void ColumnEditor::copyElement(ElementEditor *e)
{
    ElementEditor *ee = new ElementEditor();
    ee->setMode(e->mode());
    m_layout->insertWidget(m_layout->count() - 1, ee, 0, Qt::AlignTop);
    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementCopied(ElementEditor*)), this, SLOT(copyElement(ElementEditor*)));
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
                        m_layout->removeWidget(editor);
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
