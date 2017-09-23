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

#ifndef THEMECHOOSER_H
#define THEMECHOOSER_H

#include <QWidget>
#include "undoableeditor.h"

class Site;
class QGridLayout;
class MainWindow;
class Theme
{
public:
    QString name;
    QString samplePic;
    QString sampleUrl;
    bool aktiv;
};

class ThemeWidget : public QWidget
{
    Q_OBJECT
public:
    ThemeWidget(Theme *theme);

signals:
    void themeChanged(QString theme);

private slots:
    void clicked();
    void activate();

private:
    QString m_url;
    QString m_themename;
};

class ThemeChooser : public UndoableEditor
{
    Q_OBJECT

public:
    ThemeChooser(MainWindow *win, Site *site);

    void save() override;
    void load() override;

private slots:
    void themeChanged(QString themename);

private:
    QList<Theme*> m_themes;
    Site *m_site;
    MainWindow *m_win;
    QString m_themename;
};

#endif // THEMECHOOSER_H
