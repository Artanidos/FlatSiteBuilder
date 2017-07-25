#ifndef COLUMNEDITOR_H
#define COLUMNEDITOR_H

#include <QWidget>

class ColumnEditor : public QWidget
{
    Q_OBJECT

public:
    ColumnEditor();

    void mousePressEvent(QMouseEvent *event) override;
    void setContainer(QLayout *layout) {m_container = layout;}

private slots:
    void close();
    void edit();
    void copy();

private:
    QLayout *m_container;
};

#endif // COLUMNEDITOR_H
