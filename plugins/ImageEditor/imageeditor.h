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

#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QtPlugin>
#include <QObject>
#include <QComboBox>
#include "interfaces.h"
#include "imageselector.h"

class ImageEditor : public EditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.EditorInterface" FILE "imageeditor.json")
    Q_INTERFACES(EditorInterface)

public:
    ImageEditor();
    QString className() {return "ImageEditor";}
    QString displayName() {return "Image";}
    QString tagName() {return "Image";}
    QImage icon() {return QImage(":/image.png");}
    QString getHtml(QDomElement ele, QMap<QString, EditorInterface*> plugins);
    void setContent(QDomElement ele);
    QString pluginStyles();
    void activate(QString siteDir);
    void deactivate(QString siteDir);

private slots:
    void seek();
    void closeEditor();

private:
    QString m_animation;
    ImageSelector *m_image;
    QComboBox *m_animationCombo;
    QLineEdit *m_source;
    QLineEdit *m_alt;
    QLineEdit *m_title;
    QLineEdit *m_adminlabel;

    void addHeader(QString header);
};

#endif // IMAGEEDITOR_H
