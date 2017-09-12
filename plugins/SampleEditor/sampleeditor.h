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

#ifndef SAMPLEEDITOR_H
#define SAMPLEEDITOR_H

#include <QtPlugin>
#include <QObject>
#include <QLineEdit>
#include "interfaces.h"

class SampleEditor : public EditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.EditorInterface" FILE "sampleeditor.json")
    Q_INTERFACES(EditorInterface)

public:
    SampleEditor();
    QString className() {return "SampleEditor";}
    QString displayName() {return "Sample";}
    QString tagName() {return "Sample";}
    QImage icon() {return QImage(":/sample.png");}
    QString getHtml(QXmlStreamReader *xml);
    void setContent(QString content);
    QString load(QXmlStreamReader *xml) override;

private slots:
    void closeEditor();

private:
    QLineEdit *m_sampleproperty;
    QLineEdit *m_adminlabel;
};

#endif // SAMPLEEDITOR_H
