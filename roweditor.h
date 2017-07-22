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

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QGridLayout *m_layout;
};

#endif // ROWEDITOR_H
