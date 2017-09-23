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

#ifndef SITESETTINGSEDITOR_H
#define SITESETTINGSEDITOR_H

#include <QWidget>
#include "undoableeditor.h"

class MainWindow;
class Site;
class QGridLayout;
class QLineEdit;
class QComboBox;
class SiteSettingsEditor : public UndoableEditor
{
    Q_OBJECT

public:
    SiteSettingsEditor(MainWindow *win, Site *site);

    void load() override;
    void save() override;

private slots:
    void titleChanged();
    void authorChanged();
    void descriptionChanged();
    void keywordsChanged();
    void copyrightChanged();
    void publisherChanged(QString publisher);

private:
    MainWindow *m_win;
    Site *m_site;
    QLineEdit *m_title;
    QLineEdit *m_description;
    QLineEdit *m_copyright;
    QLineEdit *m_keywords;
    QLineEdit *m_author;
    QComboBox *m_publisher;
};

#endif // SITESETTINGSEDITOR_H
