/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  FlatSiteBuilder is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlatSiteBuilder is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with FlatSiteBuilder.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef PLUGINS_H
#define PLUGINS_H

#include <QMap>
#include "interfaces.h"

class Plugins
{
public:
    static ElementEditorInterface *getElementPlugin(QString name);
    static ThemeEditorInterface *getThemePlugin(QString name);
    static PublisherInterface *getPublishPlugin(QString name);
    static bool hasElementPlugin(QString name);
    static bool hasThemePlugin(QString name);
    static bool hasPublishPlugin(QString name);
    static QList<QString> elementPluginNames();
    static QList<QString> themePluginNames();
    static QList<QString> publishPluginNames();
    static void insert(QString name, ElementEditorInterface* plugin);
    static void insert(QString name, ThemeEditorInterface* plugin);
    static void insert(QString name, PublisherInterface* plugin);

    static void clearUsedPlugins() {usedPlugins.clear();}
    static void addUsedPlugin(QString plugin) {usedPlugins.append(plugin);}
    static bool isPluginUsed(QString plugin) {return usedPlugins.contains(plugin);}
    static void setActualThemeEditorPlugin(QString tep) {_actualThemeEditorPlugin = tep;}
    static QString actualThemeEditorPlugin() {return _actualThemeEditorPlugin;}
    static void setActualPublishPlugin(QString pp) {_actualPublishPlugin = pp;}
    static QString actualPublishPlugin() {return _actualPublishPlugin;}

private:
    static QMap<QString, ElementEditorInterface*> elementPlugins;
    static QMap<QString, ThemeEditorInterface*> themePlugins;
    static QMap<QString, PublisherInterface*> publishPlugins;
    static QStringList usedPlugins;
    static QString _actualThemeEditorPlugin;
    static QString _actualPublishPlugin;
};

#endif // PLUGINS_H
