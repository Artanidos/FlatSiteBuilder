#ifndef DROPAREA_H
#define DROPAREA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "hyperlink.h"

class DropArea : public QWidget
{
    Q_OBJECT

public:
    DropArea();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QVBoxLayout *m_layout;
    Hyperlink *m_link;
    QString m_normalColor;
    QString m_highlightColor;
    void setColor(QString name);
};

#endif // DROPAREA_H
