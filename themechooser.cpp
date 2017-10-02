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

#include "themechooser.h"
#include "generator.h"
#include "mainwindow.h"
#include "flatbutton.h"
#include "undoableeditor.h"
#include "plugins.h"
#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QTest>
#include <QPaintEngine>
#include <QDesktopServices>
#include <QPushButton>
#include <QScrollArea>

ThemeChooser::ThemeChooser(MainWindow *win, Site *site)
{
    m_win = win;
    m_site = site;
    m_themename = site->theme();
    m_titleLabel->setText("Theme Chooser");
    m_filename = site->sourcePath() + "/" + site->filename();
    m_themeLayout = new QGridLayout;
    QWidget *scrollContent = new QWidget;
    scrollContent->setLayout(m_themeLayout);
    QScrollArea *scroll = new QScrollArea;
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    m_layout->addWidget(scroll, 1, 0, 1, 3);

    load();
}

void ThemeChooser::themeChanged(QString themename)
{
    if(m_themename != themename)
    {
        m_themename = themename;
        contentChanged("theme changed to " + themename);
    }
}

void ThemeChooser::save()
{
    m_site->setTheme(m_themename);
    m_site->save();
    Plugins::setActualThemeEditorPlugin(m_themename);
    m_win->actualThemeChanged(m_themename);
    m_win->statusBar()->showMessage("Theme has been changed. The site should be rebuildet on the dashboard.");
    load();
}

void ThemeChooser::load()
{
    m_site->load();
    m_themename = m_site->theme();

    QDir list(Generator::themesPath());
    m_themes.clear();

    foreach(QString dir, list.entryList(QDir::NoDotAndDotDot | QDir::Dirs))
    {
        Theme *theme = new Theme();
        theme->name = dir;
        QString picName = Generator::themesPath() + "/" + dir + "/sample.png";
        QFile pic(picName);
        if(pic.exists())
            theme->samplePic = picName;
        QString htmlName = Generator::themesPath() + "/" + dir + "/sample.html";
        QFile html(htmlName);
        if(html.exists())
            theme->sampleUrl = htmlName;
        theme->aktiv = m_site->theme() == dir;
        m_themes.append(theme);
    }

    // delete the previous theme widgets
    for(int row = 0; row < m_themeLayout->rowCount(); row++)
    {
        for(int col = 0; col < m_themeLayout->columnCount(); col++)
        {
            QLayoutItem *item = m_themeLayout->itemAtPosition(row, col);
            if(item)
            {
                QWidget *w = item->widget();
                m_themeLayout->removeWidget(w);
                delete w;
            }
        }
    }

    // load the actual themes
    int row = 0;
    int col = 1;
    foreach(Theme *theme, m_themes)
    {
        ThemeWidget *tw = new ThemeWidget(theme);
        if(theme->aktiv)
        {
            m_themeLayout->addWidget(tw, 0, 0);
        }
        else
        {
            connect(tw, SIGNAL(themeChanged(QString)), this, SLOT(themeChanged(QString)));
            m_themeLayout->addWidget(tw, row, col);
            col++;
            if(col > 1)
            {
                row++;
                col = 0;
            }
        }
    }
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch();
    m_themeLayout->addLayout(vbox, row + 1, 0);
    m_win->statusBar()->showMessage("Themes have been loaded");
}

ThemeWidget::ThemeWidget(Theme *theme)
{
    m_themename = theme->name;
    setMaximumWidth(400);
    QString picurl = ":/images/theme.png";
    m_url = theme->sampleUrl;
    if(!theme->samplePic.isEmpty())
        picurl = theme->samplePic;
    FlatButton *pic = new FlatButton(picurl, picurl);
    QPixmap map = QPixmap(400, 200);
    QPainter *p = new QPainter(&map);
    p->setRenderHint(QPainter::Antialiasing);
    p->setRenderHint(QPainter::TextAntialiasing);
    p->drawImage(QRect(0,0, 400, 200), QImage(picurl));
    p->fillRect(0, 0, map.size().width(), map.size().height(), QColor(69,187,230, 125));
    if(!m_url.isEmpty())
    {
        int w = 100;
        int h = 30;

        p->fillRect(QRect((400 - w) / 2, (200 - h) / 2, w, h), QColor(69, 187, 230, 255));
        p->drawRoundedRect(QRect((400 - w) / 2, (200 - h) / 2, w, h), 5, 5);
        QFont font;
        font.setFamily("Arial");
        font.setBold(true);
        font.setPixelSize(20);
        p->setFont(font);
        p->setPen(QPen(Qt::black));
        p->drawText(QRect(0, 0, 400, 200), "PREVIEW", QTextOption(Qt::AlignHCenter|Qt::AlignVCenter));

        connect(pic, SIGNAL(clicked()), this, SLOT(clicked()));
    }
    else
        pic->setCursor(Qt::ArrowCursor);
    delete p;


    pic->setHoverPixmap(map);
    pic->setMaximumSize(400, 200);
    pic->setScaledContents(true);
    QLabel *name = new QLabel(theme->name.toUpper() + (theme->aktiv ? " (Aktiv)" : ""));
    QFont fnt = name->font();
    fnt.setPointSize(13);
    fnt.setBold(true);
    name->setFont(fnt);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pic, 0, 0, 1, 2);
    layout->addWidget(name, 1, 0);
    if(!theme->aktiv)
    {
        QPushButton *activate = new QPushButton("Activate");
        layout->addWidget(activate, 1, 1, 1, 1, Qt::AlignRight);
        connect(activate, SIGNAL(clicked(bool)), this, SLOT(activate()));
    }
    setLayout(layout);

}

void ThemeWidget::activate()
{
    emit themeChanged(m_themename);
}

void ThemeWidget::clicked()
{
    QDesktopServices::openUrl(m_url);
}
