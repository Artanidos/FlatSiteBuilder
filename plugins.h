#ifndef PLUGINS_H
#define PLUGINS_H

#include <QMap>
#include "interfaces.h"

class Plugins
{
public:
    static ElementEditorInterface *getPlugin(QString name);
    static bool hasPlugin(QString name);
    static QList<QString> pluginNames();
    static void insert(QString name, ElementEditorInterface* plugin);
    static void clearUsedPlugins() {usedPlugins.clear();}
    static void addUsedPlugin(QString plugin) {usedPlugins.append(plugin);}
    static bool isPluginUsed(QString plugin) {return usedPlugins.contains(plugin);}

private:
    static QMap<QString, ElementEditorInterface*> editorPlugins;
    static QStringList usedPlugins;
};

#endif // PLUGINS_H
