#include "globals.h"

QMap<QString, EditorInterface*> Globals::editorPlugins;

EditorInterface *Globals::getPlugin(QString name)
{
    return editorPlugins[name];
}

bool Globals::hasPlugin(QString name)
{
    return editorPlugins.contains(name);
}

QList<QString> Globals::pluginNames()
{
    return editorPlugins.keys();
}

void Globals::insert(QString name, EditorInterface* plugin)
{
    editorPlugins.insert(name, plugin);
}
