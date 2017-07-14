#include "tablecheckbox.h"

#include <QHBoxLayout>
#include <QCheckBox>

TableCheckbox::TableCheckbox()
{
    QHBoxLayout *layout = new QHBoxLayout();
    m_checkbox = new QCheckBox();
    layout->addStretch();
    layout->addWidget(m_checkbox);
    layout->addStretch();
    setLayout(layout);

    connect(m_checkbox, SIGNAL(toggled(bool)), this, SLOT(checked(bool)));
}

void TableCheckbox::checked(bool state)
{
   emit checkStateChanged(state);
}
