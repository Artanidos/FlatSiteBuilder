#include "site.h"
#include <QTest>

Site::Site()
{

}

void Site::addPage(QString title)
{
    Page *p = new Page();
    p->setTitle(title);
    m_pages.append(p);
    emit pagesChanged();
}

QQmlListProperty<Page> Site::pages()
{
    return QQmlListProperty<Page>(this, this,
             &Site::appendPage,
             &Site::pagesCount,
             &Site::page,
             &Site::clearPages);
}

void Site::appendPage(Page* p)
{
    m_pages.append(p);
}

int Site::pagesCount() const
{
    return m_pages.count();
}

Page *Site::page(int index) const
{
    return m_pages.at(index);
}

void Site::clearPages()
{
    return m_pages.clear();
}

void Site::appendPage(QQmlListProperty<Page>* list, Page* p) {
    reinterpret_cast< Site* >(list->data)->appendPage(p);
}

void Site::clearPages(QQmlListProperty<Page>* list) {
    reinterpret_cast< Site* >(list->data)->clearPages();
}

Page* Site::page(QQmlListProperty<Page>* list, int i) {
    return reinterpret_cast< Site* >(list->data)->page(i);
}

int Site::pagesCount(QQmlListProperty<Page>* list) {
    return reinterpret_cast< Site* >(list->data)->pagesCount();
}
