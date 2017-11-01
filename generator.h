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

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QMap>
#include <QDir>
#include "site.h"
#include "content.h"
#include "interfaces.h"

class Generator : public QObject
{
    Q_OBJECT
public:
    Generator();

    void generateSite(MainWindow *m_win, Site *site, Content *content = 0);

    static void setInstallDirectory(QString installDirectory) {m_installDirectory = installDirectory;}
    static QString sitesPath() {return m_installDirectory + "/sites";}
    static QString themesPath() {return m_installDirectory + "/themes";}

#ifdef TEST
    void setSitesPath(QString path) {m_sitesPath = path;}
    void setThemePath(QString path) {m_themePath = path;}
    void addThemeVar(QString name, QVariant value) {themevars[name] = value;}
    void addSiteVar(QString name, QVariant value) {sitevars[name] = value;}
    void addPageVar(QString name, QVariant value) {pagevars[name] = value;}
#endif
private:
    enum State
    {
        NormalState = -1,
        InVar,
        InLoop,
        InExpression,
    };

    enum Mode
    {
        Layout = -1,
        Include,
    };

    Site *m_site;
    MainWindow *m_win;
    QVariantMap pluginvars;
    QVariantMap pagevars;
    QVariantMap sitevars;
    QVariantMap themevars;
    QString m_themePath;
    QString m_sitesPath;
    static QString m_installDirectory;

    void copyPath(QString src, QString dst);

#ifdef TEST
public:
#endif
    QString translateContent(QString content, QVariantMap vars);
    QVariant translateVar(QString exp, QVariantMap vars);
    QString translateTemplate(QString layout, Mode mode);
    bool nextTokens(QString content, QStringList tokens);
    void generateContent(Content *content);
};

#endif // GENERATOR_H
