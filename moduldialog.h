#ifndef MODULDIALOG_H
#define MODULDIALOG_H

#include <QDialog>

class ModulDialog : public QDialog
{
    Q_OBJECT

public:
    ModulDialog();
    int result() {return m_result;}
    void setResult(int res) {m_result = res;}

private slots:
    void close1();
    void close2();
    void close3();

private:
    int m_result;
};

#endif // MODULDIALOG_H
