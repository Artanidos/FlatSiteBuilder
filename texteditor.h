#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>
#include <QTextEdit>

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    TextEditor();

    void setContent(QString content)
    {
        m_text->setHtml(content);
        m_html->setPlainText(content);
    }

signals:
    void close(QWidget*);

private slots:
    void save();
    void cancel();

private:
    QTextEdit *m_html;
    QTextEdit *m_text;
};

#endif // TEXTEDITOR_H
