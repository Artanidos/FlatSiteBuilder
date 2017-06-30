#include "column.h"

Column::Column()
{

}

QQmlListProperty<Element> Column::elements()
{
    return QQmlListProperty<Element>(this, this,
             &Column::appendElement,
             &Column::elementsCount,
             &Column::element,
             &Column::clearElements);
}

void Column::appendElement(Element* p)
{
    m_elements.append(p);
}

int Column::elementsCount() const
{
    return m_elements.count();
}

Element *Column::element(int index) const
{
    return m_elements.at(index);
}

void Column::clearElements()
{
    return m_elements.clear();
}

void Column::appendElement(QQmlListProperty<Element>* list, Element* p) {
    reinterpret_cast< Column* >(list->data)->appendElement(p);
}

void Column::clearElements(QQmlListProperty<Element>* list) {
    reinterpret_cast< Column* >(list->data)->clearElements();
}

Element* Column::element(QQmlListProperty<Element>* list, int i) {
    return reinterpret_cast< Column* >(list->data)->element(i);
}

int Column::elementsCount(QQmlListProperty<Element>* list) {
    return reinterpret_cast< Column* >(list->data)->elementsCount();
}
