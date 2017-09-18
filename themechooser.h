#ifndef THEMECHOOSER_H
#define THEMECHOOSER_H

#include <QWidget>

class Site;
class QGridLayout;
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
    void activate();

signals:
    void themeChanged(QString themename);

private:
    QString m_url;
    QString m_themename;
};

class ThemeChooser : public QWidget
{
    Q_OBJECT

public:
    ThemeChooser(Site *site);

signals:
    void contentUpdated(QString text);

private slots:
    void themeChanged(QString themename);

private:
    QList<Theme*> m_themes;
    Site *m_site;
    QGridLayout *m_layout;

    void loadThemes();
};

#endif // THEMECHOOSER_H
