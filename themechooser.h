#ifndef THEMECHOOSER_H
#define THEMECHOOSER_H

#include <QWidget>

class Site;
class QGridLayout;
class MainWindow;
class Theme
{
public:
    QString name;
    QString samplePic;
    QString sampleUrl;
    bool aktiv;
};

class ThemeWidget : public QWidget
{
    Q_OBJECT
public:
    ThemeWidget(Theme *theme);

signals:
    void themeChanged(QString theme);

private slots:
    void clicked();
    void activate();

private:
    QString m_url;
    QString m_themename;
};

class ThemeChooser : public QWidget
{
    Q_OBJECT

public:
    ThemeChooser(MainWindow *win, Site *site);

private slots:
    void themeChanged(QString themename);

private:
    QList<Theme*> m_themes;
    Site *m_site;
    QGridLayout *m_layout;
    MainWindow *m_win;

    void loadThemes();
};

#endif // THEMECHOOSER_H
