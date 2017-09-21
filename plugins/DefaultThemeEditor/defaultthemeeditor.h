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

#ifndef DEFAULTTHEMEEDITOR_H
#define DEFAULTTHEMEEDITOR_H

#include <QtPlugin>
#include <QObject>
#include <QLineEdit>
#include <QCheckBox>
#include "interfaces.h"
#include "undoableeditor.h"

class DefaultThemeEditor : public UndoableEditor, public ThemeEditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.ThemeEditorInterface" FILE "defaultthemeeditor.json")
    Q_INTERFACES(ThemeEditorInterface)

public:
    DefaultThemeEditor();
    QString className() override {return "DefaultThemeEditor";}
    QString displayName() override {return "Default Theme Editor";}
    QString themeName() override {return "default";}
    void setSourcePath(QString path) override {m_filename = path + "/DefaultTheme.xml"; load();}

    void load() override;
    void save() override;

private slots:
    void sampleChanged();
    void showPoweredChanged();

private:
    QLineEdit *m_sampleproperty;
    QCheckBox *m_showPoweredBy;
};

#endif // DEFAULTTHEMEEDITOR_H
