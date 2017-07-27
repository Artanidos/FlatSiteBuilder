#include "roweditor.h"
#include "droparea.h"
#include "columneditor.h"

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
    for(int i = 0; i < 4; i++)
    {
        ColumnEditor *ce = new ColumnEditor();
        m_layout->addWidget(ce, 0, i);
    }
    layout->addLayout(m_layout);
    setLayout(layout);
}

void RowEditor::addColumn(ColumnEditor *ce, int column)
{
    //m_layout->addWidget(ce, 0, column);
}
