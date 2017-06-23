#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include <QHash>
#include <QVariant>

class Generator : QObject
{
    Q_OBJECT
public:
    Generator();

    QString translate(QString source);
    void addVariable(QString name, QString value);

private:
    QHash<QString, QString> variables;
};

#endif // GENERATOR_H
