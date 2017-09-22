#ifndef PLUGINS_H
#define PLUGINS_H

#include <QMap>
#include "interfaces.h"

class Plugins
{
public:
    static ElementEditorInterface *getElementPlugin(QString name);
    static ThemeEditorInterface *getThemePlugin(QString name);
    static bool hasElementPlugin(QString name);
    static bool hasThemePlugin(QString name);
    static QList<QString> elementPluginNames();
    static QList<QString> themePluginNames();
    static void insert(QString name, ElementEditorInterface* plugin);
    static void insert(QString name, ThemeEditorInterface* plugin);

    static void clearUsedPlugins() {usedPlugins.clear();}
    static void addUsedPlugin(QString plugin) {usedPlugins.append(plugin);}
    static bool isPluginUsed(QString plugin) {return usedPlugins.contains(plugin);}
    static void setActualThemeEditor(QString te) {_actualThemeEditor = te;}
    static QString actualThemeEditor() {return _actualThemeEditor;}

private:
    static QMap<QString, ElementEditorInterface*> elementPlugins;
    static QMap<QString, ThemeEditorInterface*> themePlugins;
    static QStringList usedPlugins;
    static QString _actualThemeEditor;
};

#endif // PLUGINS_H
