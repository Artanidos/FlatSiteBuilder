#ifndef COLUMN_H
#define COLUMN_H

#include <QObject>
#include <QQmlListProperty>
#include <QVector>
#include "element.h"

class Column : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Element> elements READ elements)
    Q_CLASSINFO("DefaultProperty", "elements")

public:
    Column();

    QQmlListProperty<Element> elements();
    void appendElement(Element*);
    int elementsCount() const;
    Element *element(int) const;
    void clearElements();

    QString getHtml();

private:
    static void appendElement(QQmlListProperty<Element>*, Element*);
    static int elementsCount(QQmlListProperty<Element>*);
    static Element* element(QQmlListProperty<Element>*, int);
    static void clearElements(QQmlListProperty<Element>*);
    QString m_theme;
    QString m_title;
    QVector<Element *> m_elements;
};

#endif // COLUMN_H
