#include "page.h"

Page::Page()
{

}

QQmlListProperty<Section> Page::sections()
{
    return QQmlListProperty<Section>(this, this,
             &Page::appendSection,
             &Page::sectionsCount,
             &Page::section,
             &Page::clearSections);
}

void Page::appendSection(Section* p)
{
    m_sections.append(p);
}

int Page::sectionsCount() const
{
    return m_sections.count();
}

Section *Page::section(int index) const
{
    return m_sections.at(index);
}

void Page::clearSections()
{
    return m_sections.clear();
}

void Page::appendSection(QQmlListProperty<Section>* list, Section* p) {
    reinterpret_cast< Page* >(list->data)->appendSection(p);
}

void Page::clearSections(QQmlListProperty<Section>* list) {
    reinterpret_cast< Page* >(list->data)->clearSections();
}

Section* Page::section(QQmlListProperty<Section>* list, int i) {
    return reinterpret_cast< Page* >(list->data)->section(i);
}

int Page::sectionsCount(QQmlListProperty<Section>* list) {
    return reinterpret_cast< Page* >(list->data)->sectionsCount();
}
