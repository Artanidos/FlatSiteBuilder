#include "plugins.h"

QMap<QString, ElementEditorInterface*> Plugins::editorPlugins;
QStringList Plugins::usedPlugins;

ElementEditorInterface *Plugins::getPlugin(QString name)
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

void Plugins::insert(QString name, ElementEditorInterface* plugin)
{
    editorPlugins.insert(name, plugin);
}
