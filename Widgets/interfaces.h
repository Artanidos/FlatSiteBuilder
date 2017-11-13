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

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QWidget>
#include <QMap>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QUndoStack>
#include "site.h"
#include "undoableeditor.h"
#include "animateableeditor.h"

QT_BEGIN_NAMESPACE
class QString;
class QXmlStreamReader;
QT_END_NAMESPACE

class ElementEditorInterface : public AnimateableEditor
{
public:
    virtual ~ElementEditorInterface() {}
    virtual QString className() = 0;
    virtual QString displayName() = 0;
    virtual QString tagName() = 0;
    virtual QImage icon() = 0;
    virtual QString version() = 0;
    virtual QString getHtml(QXmlStreamReader *) = 0;
    virtual QString load(QXmlStreamReader *) = 0;
    virtual QString pluginStyles() {return "";}
    virtual QString pluginScripts() {return "";}
    virtual void installAssets(QString assetsPath) {Q_UNUSED(assetsPath)}
};

class ThemeEditorInterface : public UndoableEditor
{
public:
    virtual ~ThemeEditorInterface() {}
    virtual QString className() = 0;
    virtual QString displayName() = 0;
    virtual QString themeName() = 0;
    virtual QString version() = 0;
    virtual void setSourcePath(QString path) = 0;
    virtual QVariantMap themeVars() = 0;
};


class PublisherInterface : public QWidget
{
public:
    virtual ~PublisherInterface() {}
    virtual QString className() = 0;
    virtual QString displayName() = 0;
    virtual QString version() = 0;
    virtual void setSitePath(QString path) = 0;
};

QT_BEGIN_NAMESPACE
#define ElementEditorInterface_iid "com.github.Artanidos.FlatSiteBuilder.ElementEditorInterface"

Q_DECLARE_INTERFACE(ElementEditorInterface, ElementEditorInterface_iid)
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
#define ThemeEditorInterface_iid "com.github.Artanidos.FlatSiteBuilder.ThemeEditorInterface"

Q_DECLARE_INTERFACE(ThemeEditorInterface, ThemeEditorInterface_iid)
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
#define PublisherInterface_iid "com.github.Artanidos.FlatSiteBuilder.PublisherInterface"

Q_DECLARE_INTERFACE(PublisherInterface, PublisherInterface_iid)
QT_END_NAMESPACE

#endif // INTERFACES_H
