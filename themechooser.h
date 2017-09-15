#ifndef THEMECHOOSER_H
#define THEMECHOOSER_H

#include <QWidget>

class Site;
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

private slots:
    void clicked();

private:
    QString m_url;
};

class ThemeChooser : public QWidget
{
    Q_OBJECT

public:
    ThemeChooser(Site *site);

private:
    QList<Theme*> m_themes;
    Site *m_site;
    void loadThemes();
};

#endif // THEMECHOOSER_H
