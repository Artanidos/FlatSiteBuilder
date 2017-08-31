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

#ifndef MODULDIALOG_H
#define MODULDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include "interfaces.h"

class FlatButton;
class ModulDialog : public QDialog
{
    Q_OBJECT

public:
    ModulDialog();
    QString result() {return m_result;}
    void setResult(QString res) {m_result = res;}
    void registerPlugins(QMap<QString, EditorInterface*> plugins);

private slots:
    void close1();
    void close2(QString);

private:
    QString m_result;
    QGridLayout *m_grid;
    FlatButton *createButton(QImage icon, QString text);
};

#endif // MODULDIALOG_H
