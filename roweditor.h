#ifndef ROWEDITOR_H
#define ROWEDITOR_H

#include <QWidget>
#include <QGridLayout>
#include <QDomElement>
#include "elementeditor.h"
#include "flatbutton.h"

class RowEditor : public QWidget
{
    Q_OBJECT

public:
    RowEditor(bool clone = false);

    void mousePressEvent(QMouseEvent *event);
    void enableColumnAcceptDrop(bool mode);
    void addColumn(ColumnEditor *, int column, int colspan = 1);
    RowEditor* clone();
    void save(QDomDocument doc, QDomElement de);

signals:
    void beginDrag();
    void rowEditorCopied(RowEditor*);

private slots:
    void close();
    void copy();
    void edit();
    void addColumns();

private:
    QGridLayout *m_layout;
    QRect m_highlightedRect;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
    Hyperlink *m_addColumns;
};

#endif // ROWEDITOR_H
