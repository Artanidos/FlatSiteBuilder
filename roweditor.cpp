#include "roweditor.h"
#include "droparea.h"

#include <QTest>

RowEditor::RowEditor()
{
    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *widget = new QWidget();
    widget->setMinimumWidth(30);
    widget->setMaximumWidth(30);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().alternateBase().color()));
    setPalette(pal);
    setAutoFillBackground(true);

    m_highlightedRect = QRect();
    m_layout = new QGridLayout();
    ColumnEditor *ce = new ColumnEditor();
    QVBoxLayout *l = new QVBoxLayout();
    m_layout->addLayout(l, 0, 0);
    l->addWidget(ce);
    ce->setContainer(l);
    for(int i = 1; i < 4; i++)
    {
        QVBoxLayout *l = new QVBoxLayout();
        DropArea *a = new DropArea();
        m_layout->addLayout(l, 0, i);
        l->addWidget(a);
        a->setContainer(l);
    }
    layout->addWidget(widget);
    layout->addLayout(m_layout);
    setLayout(layout);
}

void RowEditor::addColumn(ColumnEditor *ce, int column)
{
    //m_layout->addWidget(ce, 0, column);
}
