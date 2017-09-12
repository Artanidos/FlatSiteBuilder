#include "plugins.h"

QMap<QString, EditorInterface*> Plugins::editorPlugins;
QStringList Plugins::usedPlugins;

EditorInterface *Plugins::getPlugin(QString name)
{
    return editorPlugins[name];
}

bool Plugins::hasPlugin(QString name)
{
    return editorPlugins.contains(name);
}

QList<QString> Plugins::pluginNames()
{
    return editorPlugins.keys();
}

void Plugins::insert(QString name, EditorInterface* plugin)
{
    editorPlugins.insert(name, plugin);
}
