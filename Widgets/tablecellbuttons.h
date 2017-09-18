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

#ifndef TABLECELLBUTTONS_H
#define TABLECELLBUTTONS_H

#include <QWidget>

class FlatButton;
class TableCellButtons : public QWidget
{
    Q_OBJECT

public:
    TableCellButtons();

    void setItem(QObject *item) {m_item = item;}

private slots:
    void deleteItemClicked();
    void editItemClicked();

signals:
    void deleteItem(QObject *);
    void editItem(QObject *);

private:
    FlatButton *m_delete;
    FlatButton *m_edit;
    QObject *m_item;
};

#endif // TABLECELLBUTTONS_H
