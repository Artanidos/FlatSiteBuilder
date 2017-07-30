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
    RowEditor();

    void mousePressEvent(QMouseEvent *event);
    void enableColumnAcceptDrop(bool mode);

signals:
    void beginDrag();

private slots:
    void close();

private:
    QGridLayout *m_layout;
    QRect m_highlightedRect;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
};

#endif // ROWEDITOR_H
