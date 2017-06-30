#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QVector>
#include <QQmlListProperty>
#include "section.h"

class Page : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(QString layout READ layout WRITE setLayout)
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

    QQmlListProperty<Section> sections();
    void appendSection(Section*);
    int sectionsCount() const;
    Section *section(int) const;
    void clearSections();

private:
    static void appendSection(QQmlListProperty<Section>*, Section*);
    static int sectionsCount(QQmlListProperty<Section>*);
    static Section* section(QQmlListProperty<Section>*, int);
    static void clearSections(QQmlListProperty<Section>*);
    QString m_title;
    QString m_url;
    QString m_layout;
    QVector<Section *> m_sections;
};

#endif // PAGE_H
