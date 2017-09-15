#include "themechooser.h"
#include "generator.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QTest>
#include <QPaintEngine>

ThemeChooser::ThemeChooser(Site *site)
{
    m_site = site;
    QLabel *titleLabel = new QLabel("Theme Chooser");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addStretch();

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(titleLabel, 0, 0);

    loadThemes();
    int row = 1;
    int col = 0;
    foreach(Theme *theme, m_themes)
    {
        ThemeWidget *tw = new ThemeWidget(theme);
        layout->addWidget(tw, row, col);
        col++;
        if(col > 1)
        {
            row++;
            col = 0;
        }
    }

    layout->addLayout(vbox, 99, 0);
    setLayout(layout);
}

ThemeWidget::ThemeWidget(Theme *theme)
{
    m_url = theme->sampleUrl;
    QPixmap map = QPixmap::fromImage(QImage(theme->samplePic));
    QPainter *p = new QPainter(&map);
    p->fillRect(0, 0, map.size().width(), map.size().height(), QColor(69,187,230, 125));
    delete p;
    FlatButton *pic = new FlatButton(theme->samplePic, theme->samplePic);
    pic->setHoverPixmap(map);
    QLabel *name = new QLabel(theme->name.toUpper() + (theme->aktiv ? " (Aktiv)" : ""));
    QFont fnt = name->font();
    fnt.setPointSize(13);
    fnt.setBold(true);
    name->setFont(fnt);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pic, 0, 0);
    layout->addWidget(name, 1, 0);
    setLayout(layout);
    connect(pic, SIGNAL(clicked()), this, SLOT(clicked()));
}

void ThemeWidget::clicked()
{
    qDebug() << m_url;
}

void ThemeChooser::loadThemes()
{
    QDir list(Generator::themesPath());

    foreach(QString dir, list.entryList(QDir::NoDotAndDotDot | QDir::Dirs))
    {
        Theme *theme = new Theme();
        theme->name = dir;
        QString picName = Generator::themesPath() + "/" + dir + "/sample-thumb.png";
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
}
