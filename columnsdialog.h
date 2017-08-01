#ifndef COLUMNSDIALOG_H
#define COLUMNSDIALOG_H

#include <QDialog>

class ColumnsDialog : public QDialog
{
    Q_OBJECT

public:
    ColumnsDialog();

    int result() {return m_result;}
    void setResult(int res) {m_result = res;}

private slots:
    void close1();
    void close2();
    void close3();
    void close4();
    void close5();
    void close6();
    void close7();
    void close8();
    void close9();
    void close10();
    void close11();

private:
    int m_result;
};

#endif // COLUMNSDIALOG_H
