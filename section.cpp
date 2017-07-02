#include "section.h"

Section::Section()
{

}

QString Section::getHtml()
{
    QString html = "<section>\n";

    for(int i = 0; i < rowsCount(); i++)
    {
        html += row(i)->getHtml();
    }
    return html + "\n</section>\n";
}

QQmlListProperty<Row> Section::rows()
{
    return QQmlListProperty<Row>(this, this,
             &Section::appendRow,
             &Section::rowsCount,
             &Section::row,
             &Section::clearRows);
}

void Section::appendRow(Row* p)
{
    m_rows.append(p);
}

int Section::rowsCount() const
{
    return m_rows.count();
}

Row *Section::row(int index) const
{
    return m_rows.at(index);
}

void Section::clearRows()
{
    return m_rows.clear();
}

void Section::appendRow(QQmlListProperty<Row>* list, Row* p) {
    reinterpret_cast< Section* >(list->data)->appendRow(p);
}

void Section::clearRows(QQmlListProperty<Row>* list) {
    reinterpret_cast< Section* >(list->data)->clearRows();
}

Row* Section::row(QQmlListProperty<Row>* list, int i) {
    return reinterpret_cast< Section* >(list->data)->row(i);
}

int Section::rowsCount(QQmlListProperty<Row>* list) {
    return reinterpret_cast< Section* >(list->data)->rowsCount();
}
