#ifndef ROW_H
#define ROW_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector>
#include "column.h"

class Row : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Column> columns READ columns)
    Q_CLASSINFO("DefaultProperty", "columns")

public:
    Row();

    QQmlListProperty<Column> columns();
    void appendColumn(Column*);
    int columnsCount() const;
    Column *column(int) const;
    void clearColumns();

    QString getHtml();

private:
    static void appendColumn(QQmlListProperty<Column>*, Column*);
    static int columnsCount(QQmlListProperty<Column>*);
    static Column* column(QQmlListProperty<Column>*, int);
    static void clearColumns(QQmlListProperty<Column>*);
    QVector<Column *> m_columns;
};

#endif // ROW_H
