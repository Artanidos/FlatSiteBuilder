#ifndef SECTIONEDITOR_H
#define SECTIONEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "roweditor.h"

class SectionEditor : public QWidget
{
    Q_OBJECT

public:
    SectionEditor();

    void addRow(RowEditor *re);

private:
    QVBoxLayout *m_layout;
};

#endif // SECTIONEDITOR_H
