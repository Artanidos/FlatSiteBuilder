#ifndef IMAGESELECTOR_H
#define IMAGESELECTOR_H

#include <QLabel>

class ImageSelector : public QLabel
{
    Q_OBJECT

public:
    ImageSelector();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked();
};

#endif // IMAGESELECTOR_H
