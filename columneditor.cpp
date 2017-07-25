#include "columneditor.h"
#include "droparea.h"
#include "columneditormimedata.h"
#include "flatbutton.h"
#include "roweditor.h"
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QTest>

ColumnEditor::ColumnEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);
    setMinimumWidth(120);
    setMinimumHeight(50);
    setMaximumHeight(50);

    FlatButton *editButton = new FlatButton(":/images/edit_normal.png", ":/images/edit_hover.png");
    FlatButton *copyButton = new FlatButton(":/images/copy_normal.png", ":/images/copy_hover.png");
    FlatButton *closeButton = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    QHBoxLayout *layout= new QHBoxLayout();
    layout->addWidget(editButton);
    layout->addWidget(copyButton);
    layout->addWidget(new QLabel("Text"), 1);
    layout->addWidget(closeButton);
    setLayout(layout);

    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void ColumnEditor::mousePressEvent(QMouseEvent *event)
{
    if(m_container->count() == 1)
    {
        DropArea *a = new DropArea();
        m_container->replaceWidget(this, a);
        a->setContainer(m_container);
    }
    else
        this->hide();

    ColumnEditorMimeData *mimeData = new ColumnEditorMimeData();
    mimeData->setData(this);

    QPixmap pixmap(this->size());
    this->render(&pixmap);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(event->pos());
    drag->setPixmap(pixmap);

    if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction))
    {
        delete this;
        return;
    }
}

void ColumnEditor::close()
{
    if(m_container->count() == 1)
    {
        DropArea *a = new DropArea();
        m_container->replaceWidget(this, a);
        a->setContainer(m_container);
    }
    else
    {
        m_container->removeWidget(this);
    }
    delete this;
}

void ColumnEditor::edit()
{
    qDebug() << "edit";
}

void ColumnEditor::copy()
{
    ColumnEditor *ce = new ColumnEditor();
    m_container->addWidget(ce);
    ce->setContainer(m_container);
}
