#ifndef SITEWIZARD_H
#define SITEWIZARD_H

#include <QWizard>

class QLabel;
class QLineEdit;
class QComboBox;
class SiteWizard : public QWizard
{
    Q_OBJECT

public:
    SiteWizard();

    void accept() override;

signals:
    void loadSite(QString file);
    void buildSite();
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

class SiteInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    SiteInfoPage(QWidget *parent = 0);

private:
    QLabel *m_siteNameLabel;
    QLineEdit *m_siteNameLineEdit;
    QLabel *m_copyrightLabel;
    QLineEdit *m_copyrightLineEdit;
    QLabel *m_descriptionLabel;
    QLineEdit *m_descriptionLineEdit;
    QLabel *m_themeLabel;
    QComboBox *m_theme;

};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage() override;

private:
    QLabel *label;
};

#endif // SITEWIZARD_H
