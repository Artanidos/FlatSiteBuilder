#ifndef ROWEDITOR_H
#define ROWEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include "columneditor.h"

class RowEditor : public QWidget
{
    Q_OBJECT

public:
    RowEditor();

    void addColumn(ColumnEditor *ce, int column);

private:
    QGridLayout *m_layout;
    QRect m_highlightedRect;
};

#endif // ROWEDITOR_H
