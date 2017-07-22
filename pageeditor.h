#ifndef PAGEEDITOR_H
#define PAGEEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "sectioneditor.h"

class PageEditor : public QWidget
{
public:
    PageEditor();

    void addSection(SectionEditor *se);

private:
    QVBoxLayout *m_layout;
};

#endif // PAGEEDITOR_H
