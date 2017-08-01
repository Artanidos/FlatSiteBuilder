#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QWidget>

class TextEditor : public QWidget
{
    Q_OBJECT

public:
    TextEditor();

signals:
    void close(QWidget*);

private slots:
    void save();
    void cancel();
};

#endif // TEXTEDITOR_H
