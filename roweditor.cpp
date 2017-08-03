#include "roweditor.h"
#include "columneditor.h"
#include "widgetmimedata.h"
#include "sectioneditor.h"
#include "pageeditor.h"
#include "columnsdialog.h"

#include <QTest>
#include <QDrag>

RowEditor::RowEditor(bool clone)
{
    m_editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    m_closeButton = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");

    m_addColumns = new Hyperlink("(+) Add Columns");
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setAlignment(Qt::AlignTop);
    vbox->setSpacing(5);
    vbox->addWidget(m_editButton);
    vbox->addWidget(m_copyButton);
    vbox->addWidget(m_closeButton, 0, Qt::AlignBottom);
    QHBoxLayout *layout = new QHBoxLayout();

    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().alternateBase().color()));
    setPalette(pal);
    setAutoFillBackground(true);

    m_highlightedRect = QRect();
    m_layout = new QGridLayout();
    if(!clone)
    {
        QHBoxLayout *s1 = new QHBoxLayout();
        s1->addStretch();
        QHBoxLayout *s2 = new QHBoxLayout();
        s2->addStretch();
        m_layout->addLayout(s1, 0, 0);
        m_layout->addWidget(m_addColumns, 0, 1, 1, 1, Qt::AlignCenter);
        m_layout->addLayout(s2, 0, 2);
    }
    layout->addItem(vbox);
    layout->addLayout(m_layout);
    setLayout(layout);

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(m_editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(m_addColumns, SIGNAL(linkActivated(QString)), this, SLOT(addColumns()));
}

void RowEditor::save(QDomDocument doc, QDomElement de)
{
    QDomElement row = doc.createElement("Row");
    for(int i = 0; i < m_layout->count(); i++)
    {
        ColumnEditor *ce = dynamic_cast<ColumnEditor*>(m_layout->itemAt(i)->widget());
        if(ce)
            ce->save(doc, row);
    }
    de.appendChild(row);
}

void RowEditor::close()
{
    delete this;
}

void RowEditor::copy()
{
    emit rowEditorCopied(this);
}

void RowEditor::edit()
{
    qDebug() << "edit";
}

void RowEditor::addColumns()
{
    ColumnsDialog *dlg = new ColumnsDialog();
    dlg->exec();
    if(dlg->result() == 0)
        return;

    m_layout->removeItem(m_layout->itemAt(2));
    m_layout->removeItem(m_layout->itemAt(0));
    m_layout->removeWidget(m_addColumns);
    delete m_addColumns;

    switch(dlg->result())
    {
        case 1: // 1/1
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0);
            break;
        }
        case 2: // 1/2 - 1/2
        {
            for(int i = 0; i < 2; i++)
            {
                ColumnEditor *ce = new ColumnEditor();
                addColumn(ce, i);
            }
            break;
        }
        case 3: // 1/3 - 1/3 - 1/3
        {
            for(int i = 0; i < 3; i++)
            {
                ColumnEditor *ce = new ColumnEditor();
                addColumn(ce, i);
            }
            break;
        }
        case 4: // 1/4 - 1/4 - 1/4 - 1/4
        {
            for(int i = 0; i < 4; i++)
            {
                ColumnEditor *ce = new ColumnEditor();
                addColumn(ce, i);
            }
            break;
        }
        case 5: // 2/3 - 1/3
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0, 2);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 2);
            break;
        }
        case 6: // 1/3 - 2/3
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 1, 2);
            break;
        }
        case 7: // 1/4 - 3/4
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 1, 3);
            break;
        }
        case 8: // 3/4 - 1/4
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0, 3);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 1);
            break;
        }
        case 9: // 1/2 - 1/4 - 1/4
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0, 2);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 1);
            ColumnEditor *ce3 = new ColumnEditor();
            addColumn(ce3, 2);
            break;
        }
        case 10: // 1/4 - 1/4 - 1/2
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 1);
            ColumnEditor *ce3 = new ColumnEditor();
            addColumn(ce3, 2, 2);
            break;
        }
        case 11: // 1/4 - 1/2 - 1/4
        {
            ColumnEditor *ce = new ColumnEditor();
            addColumn(ce, 0);
            ColumnEditor *ce2 = new ColumnEditor();
            addColumn(ce2, 1, 2);
            ColumnEditor *ce3 = new ColumnEditor();
            addColumn(ce3, 2);
            break;
        }
    }
}

void RowEditor::addColumn(ColumnEditor *ce, int column, int stretch)
{
    m_layout->addWidget(ce, 0, column);
    m_layout->setColumnStretch(column, stretch);
}

RowEditor* RowEditor::clone()
{
    RowEditor * nre = new RowEditor(true);
    for(int i = 0; i < m_layout->count(); i++)
    {
        ColumnEditor *ce = dynamic_cast<ColumnEditor*>(m_layout->itemAt(i)->widget());
        if(ce)
            nre->addColumn(ce->clone(), i);
    }
    return nre;
}

void RowEditor::enableColumnAcceptDrop(bool mode)
{
    for(int i = 0; i < m_layout->count(); i++)
    {
        ColumnEditor *ce = dynamic_cast<ColumnEditor*>(m_layout->itemAt(i)->widget());
        if(ce)
            ce->setAcceptDrops(mode);
    }
}

void RowEditor::mousePressEvent(QMouseEvent *event)
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

    SectionEditor *se = dynamic_cast<SectionEditor*>(parentWidget());
    se->removeRow(this);
    PageEditor *pe = dynamic_cast<PageEditor*>(se->parentWidget());
    pe->enableColumnAcceptDrop(false);
    this->hide();

    if(drag->exec(Qt::MoveAction) == Qt::IgnoreAction)
    {
        se->addRow(this);
        this->show();
    }
    pe->enableColumnAcceptDrop(true);
}
