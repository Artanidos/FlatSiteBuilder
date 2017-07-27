#include "columneditor.h"
#include "elementeditor.h"
#include "elementeditormimedata.h"
#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QTest>

ColumnEditor::ColumnEditor()
{
    m_layout = new QVBoxLayout();
    m_layout->setAlignment(Qt::AlignTop);
    setLayout(m_layout);
    setAcceptDrops(true);

    ElementEditor *ee = new ElementEditor();
    m_layout->addWidget(ee, 0, Qt::AlignTop);

    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
}

void ColumnEditor::addElement()
{
    ElementEditor *ee = new ElementEditor();
    m_layout->addWidget(ee, 0, Qt::AlignTop);

    connect(ee, SIGNAL(elementEnabled()), this, SLOT(addElement()));
    connect(ee, SIGNAL(elementDragged()), this, SLOT(addElement()));
}

void ColumnEditor::dragEnterEvent(QDragEnterEvent *event)
{
    const ElementEditorMimeData *myData = qobject_cast<const ElementEditorMimeData *>(event->mimeData());
    if(myData)
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
    const ElementEditorMimeData *myData = qobject_cast<const ElementEditorMimeData *>(event->mimeData());
    if(myData)
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
    {
        event->ignore();
    }
}

void ColumnEditor::dropEvent(QDropEvent *event)
{
    const ElementEditorMimeData *myData = qobject_cast<const ElementEditorMimeData *>(event->mimeData());
    if (myData)
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

        ElementEditor *ee = myData->getData();
        m_layout->insertWidget(0, ee, 0, Qt::AlignTop);
        ee->dropped();
        ee->show();
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
