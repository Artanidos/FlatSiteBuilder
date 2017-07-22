#include "pageeditor.h"
#include <QVBoxLayout>

PageEditor::PageEditor()
{
    m_layout = new QVBoxLayout();
    m_layout->addStretch();
    setLayout(m_layout);
}

void PageEditor::addSection(SectionEditor *se)
{
    m_layout->insertWidget(m_layout->count() - 1, se);
}
