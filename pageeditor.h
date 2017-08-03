#ifndef PAGEEDITOR_H
#define PAGEEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "sectioneditor.h"

class PageEditor : public QWidget
{
    Q_OBJECT

public:
    PageEditor();

    void addSection(SectionEditor *se);
    void removeSection(SectionEditor *se);
    void enableColumnAcceptDrop(bool mode);
    void enableSectionAcceptDrop(bool mode);
    QList<SectionEditor*> sections();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void addSection();
    void copySection(SectionEditor *);

private:
    QVBoxLayout *m_layout;
};

#endif // PAGEEDITOR_H
