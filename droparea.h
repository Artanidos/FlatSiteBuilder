#ifndef DROPAREA_H
#define DROPAREA_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include "hyperlink.h"

class DropArea : public QWidget
{
    Q_OBJECT

public:
    DropArea();
    void setContainer(QVBoxLayout *layout) {m_container = layout;}

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QHBoxLayout *m_layout;
    Hyperlink *m_link;
    QString m_normalColor;
    QString m_highlightColor;
    void setColor(QString name);
    QVBoxLayout *m_container;
};

#endif // DROPAREA_H
