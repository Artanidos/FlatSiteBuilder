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

#ifndef INSTALLDIALOG_H
#define INSTALLDIALOG_H

#include <QDialog>

class QLineEdit;
class InstallDialog : public QDialog
{
    Q_OBJECT

public:
    InstallDialog();

    QString installDirectory() {return m_installDirectory;}

private slots:
    void installClicked();
    void cancelClicked();
    void chooseClicked();

private:
    QString m_installDirectory;
    QLineEdit *m_path;
};

#endif // INSTALLDIALOG_H
