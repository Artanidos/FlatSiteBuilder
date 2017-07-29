#include "roweditor.h"
#include "columneditor.h"
#include "widgetmimedata.h"
#include "sectioneditor.h"

#include <QTest>
#include <QDrag>

RowEditor::RowEditor()
{
    m_editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    m_copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    m_closeButton = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");

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
    for(int i = 0; i < 4; i++)
    {
        ColumnEditor *ce = new ColumnEditor();
        m_layout->addWidget(ce, 0, i);
    }
    layout->addItem(vbox);
    layout->addLayout(m_layout);
    setLayout(layout);

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void RowEditor::close()
{
    delete this;
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
    this->hide();

    if(drag->exec(Qt::MoveAction) == Qt::IgnoreAction)
    {
        se->addRow(this);
        this->show();
    }
}