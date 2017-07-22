#include "sectioneditor.h"

SectionEditor::SectionEditor()
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(palette().base().color().name()));
    setPalette(pal);
    setAutoFillBackground(true);
    setMaximumHeight(200);

    m_layout = new QVBoxLayout();
    m_layout->addStretch();
    setLayout(m_layout);
}

void SectionEditor::addRow(RowEditor *re)
{
    m_layout->insertWidget(m_layout->count() - 1, re);
}
