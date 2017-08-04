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

#include "textdialog.h"
#include "htmlhighlighter.h"
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QDesktopWidget>

TextDialog::TextDialog()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(13);

    setWindowTitle("Edit Text");
    QGridLayout *grid = new QGridLayout();
    QTabWidget *tab = new QTabWidget();
    QTextEdit *text = new QTextEdit();
    QTextEdit *html = new QTextEdit();
    html->setFont(font);
    html->setAcceptRichText(false);
    html->setLineWrapMode(QTextEdit::NoWrap);
    QFontMetrics metrics(font);
    html->setTabStopWidth(4 * metrics.width(' '));
    new HtmlHighlighter(html->document());

    tab->addTab(text, "Visual");
    tab->addTab(html, "HTML");
    grid->addWidget(tab, 0, 0);

    QPushButton *okButton = new QPushButton("Ok");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    //mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    readSettings();
}

void TextDialog::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void TextDialog::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("textdialog.geometry", saveGeometry());
    //settings.setValue("textdialog.state", saveState());

}

void TextDialog::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("textdialog.geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }
    else
    {
        restoreGeometry(geometry);
        //restoreState(settings.value("textdialog.state").toByteArray());
    }
}
