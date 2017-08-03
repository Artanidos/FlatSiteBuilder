#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>
#include <QTextEdit>

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    TextEditor();

    void setContent(QString content) {m_html->setPlainText(content);}
    QString content() {return m_html->toPlainText();}
    bool changed() {return m_changed;}

signals:
    void close(QWidget*);

private slots:
    void save();
    void cancel();
    void textChanged();

private:
    QTextEdit *m_html;
    bool m_changed;
};

#endif // TEXTEDITOR_H
