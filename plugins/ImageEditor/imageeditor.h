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
#include "interfaces.h"
#include "imageselector.h"

class QLineEdit;
class ImageEditor : public ElementEditorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.github.Artanidos.FlatSiteBuilder.EditorInterface" FILE "imageeditor.json")
    Q_INTERFACES(ElementEditorInterface)

public:
    ImageEditor();

    QString className() {return "ImageEditor";}
    QString displayName() {return "Image";}
    QString tagName() {return "Image";}
    QImage icon() {return QImage(":/image.png");}
    QString getHtml(QXmlStreamReader *xml);
    void setContent(QString content);
    QString load(QXmlStreamReader *xml) override;

private slots:
    void seek();
    void closeEditor();

private:
    QString m_animation;
    ImageSelector *m_image;
    QLineEdit *m_source;
    QLineEdit *m_alt;
    QLineEdit *m_title;
    QLineEdit *m_adminlabel;
    void toString();
};

#endif // IMAGEEDITOR_H
