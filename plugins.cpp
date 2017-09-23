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

#include "plugins.h"

QMap<QString, ElementEditorInterface*> Plugins::elementPlugins;
QMap<QString, ThemeEditorInterface*> Plugins::themePlugins;
QMap<QString, PublisherInterface*> Plugins::publishPlugins;
QStringList Plugins::usedPlugins;
QString Plugins::_actualThemeEditorPlugin;
QString Plugins::_actualPublishPlugin;

ElementEditorInterface *Plugins::getElementPlugin(QString name)
{
    return elementPlugins[name];
}

ThemeEditorInterface *Plugins::getThemePlugin(QString name)
{
    return themePlugins[name];
}

PublisherInterface *Plugins::getPublishPlugin(QString name)
{
    return publishPlugins[name];
}

bool Plugins::hasElementPlugin(QString name)
{
    return elementPlugins.contains(name);
}

bool Plugins::hasThemePlugin(QString name)
{
    return themePlugins.contains(name);
}

bool Plugins::hasPublishPlugin(QString name)
{
    return publishPlugins.contains(name);
}

QList<QString> Plugins::elementPluginNames()
{
    return elementPlugins.keys();
}

QList<QString> Plugins::themePluginNames()
{
    return themePlugins.keys();
}

QList<QString> Plugins::publishPluginNames()
{
    return publishPlugins.keys();
}

void Plugins::insert(QString name, ElementEditorInterface* plugin)
{
    elementPlugins.insert(name, plugin);
}

void Plugins::insert(QString name, ThemeEditorInterface *plugin)
{
    themePlugins.insert(name, plugin);
}

void Plugins::insert(QString name, PublisherInterface *plugin)
{
    publishPlugins.insert(name, plugin);
}
