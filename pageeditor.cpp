#include "pageeditor.h"
#include <QVBoxLayout>

PageEditor::PageEditor()
{
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setAlignment(Qt::AlignLeft);
    hbox->setSpacing(10);
    Hyperlink *addSection = new Hyperlink("(+) Add Section");
    Hyperlink *addFullSection = new Hyperlink("(+) Add Full Width Section");
    m_layout = new QVBoxLayout();
    hbox->addWidget(addSection);
    hbox->addWidget(addFullSection);
    m_layout->addLayout(hbox);
    m_layout->addStretch();
    setLayout(m_layout);

    connect(addSection, SIGNAL(linkActivated(QString)), this, SLOT(addSection()));
}

void PageEditor::addSection(SectionEditor *se)
{
    m_layout->insertWidget(m_layout->count() - 2, se);
}

void PageEditor::addSection()
{
    addSection(new SectionEditor());
}
