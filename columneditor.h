#ifndef COLUMNEDITOR_H
#define COLUMNEDITOR_H

#include <QWidget>

class ColumnEditor : public QWidget
{
    Q_OBJECT

public:
    ColumnEditor();

    void mousePressEvent(QMouseEvent *event) override;
};

#endif // COLUMNEDITOR_H
