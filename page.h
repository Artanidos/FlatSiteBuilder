#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QVector>
#include <QQmlListProperty>
#include "section.h"

class Page : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    Q_PROPERTY(QString layout READ layout WRITE setLayout NOTIFY layoutChanged)
    Q_PROPERTY(QQmlListProperty<Section> sections READ sections)
    Q_CLASSINFO("DefaultProperty", "sections")

public:
    Page();

    QString title() {return m_title;}
    void setTitle(QString title) {m_title = title;}

    QString url() {return m_url;}
    void setUrl(QString url) {m_url = url;}

    QString layout() {return m_layout;}
    void setLayout(QString layout) {m_layout = layout;}

    QString author() {return m_author;}
    void setAuthor(QString author) {m_author = author;}

    QQmlListProperty<Section> sections();
    void appendSection(Section*);
    int sectionsCount() const;
    Section *section(int) const;
    void clearSections();

    QString getHtml();

private:
    static void appendSection(QQmlListProperty<Section>*, Section*);
    static int sectionsCount(QQmlListProperty<Section>*);
    static Section* section(QQmlListProperty<Section>*, int);
    static void clearSections(QQmlListProperty<Section>*);
    QString m_title;
    QString m_url;
    QString m_layout;
    QString m_author;
    QVector<Section *> m_sections;

signals:
    void authorChanged();
    void titleChanged();
    void urlChanged();
    void layoutChanged();
};

#endif // PAGE_H
