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

#ifndef COLUMNSDIALOG_H
#define COLUMNSDIALOG_H

#include <QDialog>

class ColumnsDialog : public QDialog
{
    Q_OBJECT

public:
    ColumnsDialog();

    int result() {return m_result;}
    void setResult(int res) {m_result = res;}

private slots:
    void close1();
    void close2();
    void close3();
    void close4();
    void close5();
    void close6();
    void close7();
    void close8();
    void close9();
    void close10();
    void close11();

private:
    int m_result;
};

#endif // COLUMNSDIALOG_H
