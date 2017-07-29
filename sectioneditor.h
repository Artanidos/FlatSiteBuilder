#ifndef SECTIONEDITOR_H
#define SECTIONEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "roweditor.h"
#include "flatbutton.h"

class SectionEditor : public QWidget
{
    Q_OBJECT

public:
    SectionEditor();

    void addRow(RowEditor *re);
    void removeRow(RowEditor *re);

private slots:
    void addRow();
    void close();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QVBoxLayout *m_layout;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
};

#endif // SECTIONEDITOR_H
