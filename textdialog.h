#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>

class TextDialog : public QDialog
{
public:
    TextDialog();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void writeSettings();
    void readSettings();
};

#endif // TEXTDIALOG_H
