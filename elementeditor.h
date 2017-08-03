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
    void setText(QString text) {m_text->setText(text);}
    void setContent(QString content) {m_content = content;}
    QString content() {return m_content;}
    enum Mode {Empty, Enabled, Dropzone};
    void setMode(Mode mode);
    Mode mode() {return m_mode;}
    void save(QDomDocument doc, QDomElement de);

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
    QString m_content;
    Mode m_mode;
    bool m_zoom;

    void setColor(QString name);
};

#endif // ELEMENTEDITOR_H
