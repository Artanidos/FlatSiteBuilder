#include "plugins.h"

QMap<QString, ElementEditorInterface*> Plugins::elementPlugins;
QMap<QString, ThemeEditorInterface*> Plugins::themePlugins;
QStringList Plugins::usedPlugins;

ElementEditorInterface *Plugins::getElementPlugin(QString name)
{
    return elementPlugins[name];
}

ThemeEditorInterface *Plugins::getThemePlugin(QString name)
{
    return themePlugins[name];
}

bool Plugins::hasElementPlugin(QString name)
{
    return elementPlugins.contains(name);
}

bool Plugins::hasThemePlugin(QString name)
{
    return themePlugins.contains(name);
}

QList<QString> Plugins::elementPluginNames()
{
    return elementPlugins.keys();
}

QList<QString> Plugins::themePluginNames()
{
    return themePlugins.keys();
}

void Plugins::insert(QString name, ElementEditorInterface* plugin)
{
    elementPlugins.insert(name, plugin);
}

void Plugins::insert(QString name, ThemeEditorInterface *plugin)
{
    themePlugins.insert(name, plugin);
}
