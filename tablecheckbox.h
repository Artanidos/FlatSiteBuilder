#ifndef TABLECHECKBOX_H
#define TABLECHECKBOX_H

#include <QWidget>
#include <QCheckBox>

class TableCheckbox : public QWidget
{
    Q_OBJECT
public:
    TableCheckbox();

    Qt::CheckState checked() {return m_checkbox->checkState();}
    void setChecked(bool state) {m_checkbox->setChecked(state);}

private slots:
    void checked(bool state);

signals:
    void checkStateChanged(bool state);

private:
    QCheckBox *m_checkbox;
};

#endif // TABLECHECKBOX_H
