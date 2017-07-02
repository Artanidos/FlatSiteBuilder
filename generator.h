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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "PythonQt.h"
#include "PythonQt_QtAll.h"

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QMap>
#include "site.h"

class Generator : QObject
{
    Q_OBJECT
public:
    Generator();

    void generateSite(Site *site, QString path);

private:
    Site *m_site;
    QVariantMap globals;
    QVariantMap pagevars;
    QVariantMap sitevars;
    PythonQtObjectPtr context;

    void parseFront(QString content);
    QString translateContent(QString content);
    QString translateMarkdown(QString content);
    QVariantMap parseYaml(QString code);
    void copyPath(QString src, QString dst);
};

#endif // GENERATOR_H
