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
    SiteWizard(QString installDirectory);

    void accept() override;

signals:
    void loadSite(QString file);
    void buildSite();

private:
    QString m_installDirectory;
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
    SiteInfoPage(QString installDirectory, QWidget *parent = 0);

private:
    QLabel *m_siteNameLabel;
    QLineEdit *m_siteNameLineEdit;
    QLabel *m_copyrightLabel;
    QLineEdit *m_copyrightLineEdit;
    QLabel *m_descriptionLabel;
    QLineEdit *m_descriptionLineEdit;
    QLabel *m_themeLabel;
    QComboBox *m_theme;
    QLabel *m_warning;
    QString m_installDirectory;

private slots:
    void siteNameChanged(QString name);
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
