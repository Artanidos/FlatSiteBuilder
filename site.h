#ifndef SITE_H
#define SITE_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QQmlListProperty>
#include "page.h"

class Site : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QQmlListProperty<Page> pages READ pages NOTIFY pagesChanged)

public:
    Site();

    QString theme() {return m_theme;}
    void setTheme(QString theme) {m_theme = theme;}

    QString title() {return m_title;}
    void setTitle(QString title) {m_title = title;}

    QQmlListProperty<Page> pages();
    void appendPage(Page*);
    int pagesCount() const;
    Page *page(int) const;
    void clearPages();

private:
    static void appendPage(QQmlListProperty<Page>*, Page*);
    static int pagesCount(QQmlListProperty<Page>*);
    static Page* page(QQmlListProperty<Page>*, int);
    static void clearPages(QQmlListProperty<Page>*);
    QString m_theme;
    QString m_title;
    QVector<Page *> m_pages;

signals:
    void themeChanged();
    void titleChanged();
    void pagesChanged();
};

#endif // SITE_H
