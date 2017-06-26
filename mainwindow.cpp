/****************************************************************************
** Copyright (C) 2017 Olaf Japp
**
** This file is part of FlatSiteBuilder.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "mainwindow.h"

#include <QCloseEvent>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QDir>

#include "generator.h"
#include "PythonQt.h"
#include "PythonQt_QtAll.h"

MainWindow::MainWindow()
{
    readSettings();
    initPython();

    Generator gen;

    gen.generateSite("/home/olaf/SourceCode/FlatSiteBuilder/testsite");
}

void MainWindow::initPython()
{
    PythonQt::init();
    PythonQt_QtAll::init();
    connect(PythonQt::self(), SIGNAL(pythonStdOut(QString)), this, SLOT(OnPythonQtStdOut(QString)));
    connect(PythonQt::self(), SIGNAL(pythonStdErr(QString)), this, SLOT(OnPythonQtStdErr(QString)));
    PythonQt::self()->addSysPath("/home/olaf/.local/lib/python2.7/site-packages/");
    PythonQt::self()->addSysPath("/usr/local/lib/python2.7/dist-packages/");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
        restoreState(settings.value("state").toByteArray());
    }
}

void MainWindow::OnPythonQtStdOut(QString str)
{
    qDebug() << "Python:" << str;
}

void MainWindow::OnPythonQtStdErr(QString str)
{
    qDebug() << "PythonErr:" << str;
}
