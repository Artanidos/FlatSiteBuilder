#ifndef ROWEDITOR_H
#define ROWEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include "elementeditor.h"
#include "flatbutton.h"

class RowEditor : public QWidget
{
    Q_OBJECT

public:
    RowEditor(bool clone = false);

    void mousePressEvent(QMouseEvent *event);
    void enableColumnAcceptDrop(bool mode);
    void addColumn(ColumnEditor *, int);
    RowEditor* clone();

signals:
    void beginDrag();
    void rowEditorCopied(RowEditor*);

private slots:
    void close();
    void copy();
    void edit();

private:
    QGridLayout *m_layout;
    QRect m_highlightedRect;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
};

#endif // ROWEDITOR_H
