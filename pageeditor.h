#ifndef PAGEEDITOR_H
#define PAGEEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "sectioneditor.h"

class PageEditor : public QWidget
{
    Q_OBJECT

public:
    PageEditor();

    void addSection(SectionEditor *se);

private slots:
    void addSection();

private:
    QVBoxLayout *m_layout;
};

#endif // PAGEEDITOR_H
