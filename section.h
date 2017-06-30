#ifndef SECTION_H
#define SECTION_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector>
#include "row.h"

class Section : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Row> rows READ rows)
    Q_CLASSINFO("DefaultProperty", "rows")

public:
    Section();

    QQmlListProperty<Row> rows();
    void appendRow(Row*);
    int rowsCount() const;
    Row *row(int) const;
    void clearRows();

private:
    static void appendRow(QQmlListProperty<Row>*, Row*);
    static int rowsCount(QQmlListProperty<Row>*);
    static Row* row(QQmlListProperty<Row>*, int);
    static void clearRows(QQmlListProperty<Row>*);
    QVector<Row*> m_rows;
};

#endif // SECTION_H
