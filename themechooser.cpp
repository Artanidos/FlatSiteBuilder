#include "themechooser.h"
#include "generator.h"
#include "mainwindow.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QStatusBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QTest>
#include <QPaintEngine>
#include <QDesktopServices>
#include <QPushButton>

ThemeChooser::ThemeChooser(MainWindow *win, Site *site)
{
    m_win = win;
    m_site = site;
    QLabel *titleLabel = new QLabel("Theme Chooser");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch();

    m_layout = new QGridLayout;
    m_layout->addWidget(titleLabel, 0, 0);

    loadThemes();

    m_layout->addLayout(vbox, m_layout->rowCount(), 0);
    setLayout(m_layout);
}

void ThemeChooser::themeChanged(QString themename)
{
    m_site->setTheme(themename);
    m_site->save();

    loadThemes();

    m_win->statusBar()->showMessage("The project should be rebuildet.");
}

void ThemeChooser::loadThemes()
{
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

    // delete the previous themes
    for(int row = 1; row < m_layout->rowCount(); row++)
    {
        for(int col = 0; col < m_layout->columnCount(); col++)
        {
            QLayoutItem *item = m_layout->itemAtPosition(row, col);
            if(item)
            {
                ThemeWidget *tw = dynamic_cast<ThemeWidget*>(item->widget());
                if(tw)
                {
                    m_layout->removeWidget(tw);
                    delete tw;
                }
            }
        }
    }

    // load the actual themes
    int row = 1;
    int col = 1;
    foreach(Theme *theme, m_themes)
    {
        ThemeWidget *tw = new ThemeWidget(theme);
        if(theme->aktiv)
        {
            m_layout->addWidget(tw, 1, 0);
        }
        else
        {
            connect(tw, SIGNAL(themeChanged(QString)), this, SLOT(themeChanged(QString)));
            m_layout->addWidget(tw, row, col);
            col++;
            if(col > 1)
            {
                row++;
                col = 0;
            }
        }
    }
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
