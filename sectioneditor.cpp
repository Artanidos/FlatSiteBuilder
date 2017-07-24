#include "sectioneditor.h"
#include <QTest>

SectionEditor::SectionEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);

    QHBoxLayout *layout = new QHBoxLayout();
    QWidget *widget = new QWidget();
    widget->setMinimumWidth(30);
    widget->setMaximumWidth(30);
    m_layout = new QVBoxLayout();
    layout->addWidget(widget);
    layout->addLayout(m_layout);
    setLayout(layout);
}

void SectionEditor::addRow(RowEditor *re)
{
    m_layout->insertWidget(m_layout->count(), re);
}
