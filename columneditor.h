#ifndef COLUMNEDITOR_H
#define COLUMNEDITOR_H

#include <QWidget>
#include <QVBoxLayout>

class ElementEditor;
class ColumnEditor : public QWidget
{
    Q_OBJECT

public:
    ColumnEditor();

    ColumnEditor* clone();
    void addElement(ElementEditor *);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

public slots:
    void addElement();
    void copyElement(ElementEditor*);

private:
    QVBoxLayout *m_layout;
};

#endif // COLUMNEDITOR_H
