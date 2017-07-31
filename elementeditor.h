#ifndef ELEMENTEDITOR_H
#define ELEMENTEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include "columneditor.h"
#include "hyperlink.h"
#include "flatbutton.h"

class ElementEditor : public QWidget
{
    Q_OBJECT

public:
    ElementEditor();

    void mousePressEvent(QMouseEvent *event) override;

    ElementEditor *clone();
    enum Mode {Empty, Enabled, Dropzone};
    void setMode(Mode mode);
    Mode mode() {return m_mode;}

public slots:
    void dropped();

private slots:
    void enable();
    void close();
    void edit();
    void copy();

signals:
    void elementEnabled();
    void elementDragged();
    void elementCopied(ElementEditor*);

private:
    Hyperlink *m_link;
    FlatButton *m_editButton;
    FlatButton *m_copyButton;
    FlatButton *m_closeButton;
    QLabel *m_text;
    QString m_normalColor;
    QString m_enabledColor;
    QString m_dropColor;
    Mode m_mode;

    void setColor(QString name);
};

#endif // ELEMENTEDITOR_H
