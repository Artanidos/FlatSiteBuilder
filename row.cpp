#include "row.h"

Row::Row()
{

}

QQmlListProperty<Column> Row::columns()
{
    return QQmlListProperty<Column>(this, this,
             &Row::appendColumn,
             &Row::columnsCount,
             &Row::column,
             &Row::clearColumns);
}

void Row::appendColumn(Column* p)
{
    m_columns.append(p);
}

int Row::columnsCount() const
{
    return m_columns.count();
}

Column *Row::column(int index) const
{
    return m_columns.at(index);
}

void Row::clearColumns()
{
    return m_columns.clear();
}

void Row::appendColumn(QQmlListProperty<Column>* list, Column* p) {
    reinterpret_cast< Row* >(list->data)->appendColumn(p);
}

void Row::clearColumns(QQmlListProperty<Column>* list) {
    reinterpret_cast< Row* >(list->data)->clearColumns();
}

Column* Row::column(QQmlListProperty<Column>* list, int i) {
    return reinterpret_cast< Row* >(list->data)->column(i);
}

int Row::columnsCount(QQmlListProperty<Column>* list) {
    return reinterpret_cast< Row* >(list->data)->columnsCount();
}
