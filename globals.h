#ifndef GLOBALS_H
#define GLOBALS_H

#include <QMap>
#include "interfaces.h"

class Globals
{
public:
    static EditorInterface *getPlugin(QString name);
    static bool hasPlugin(QString name);
    static QList<QString> pluginNames();
    static void insert(QString name, EditorInterface* plugin);

private:
    static QMap<QString, EditorInterface*> editorPlugins;
};

#endif // GLOBALS_H
