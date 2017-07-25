#ifndef FLATBUTTON_H
#define FLATBUTTON_H

#include <QLabel>

class FlatButton : public QLabel
{
    Q_OBJECT

public:
    FlatButton(QString normal, QString hover);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent * event);
    void leaveEvent(QEvent * event);

signals:
    void clicked();

private:
    QImage m_normalIcon;
    QImage m_hoverIcon;
};

#endif // FLATBUTTON_H
