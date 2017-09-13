#include "sitewizard.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QXmlStreamWriter>
#include <QTest>

SiteWizard::SiteWizard()
{
    addPage(new IntroPage);
    addPage(new SiteInfoPage);
    addPage(new ConclusionPage);

    setWindowTitle(tr("Site Wizard"));
}

void SiteWizard::accept()
{
    QString siteName = field("siteName").toString();
    QString description = field("description").toString();
    QString copyright = field("copyright").toString();
    QString path = QDir::homePath() + "/FlatSiteBuilder/sources/" + siteName.toLower();

    QDir dir(QDir::homePath() + "/FlatSiteBuilder/sources/");
    dir.mkdir(siteName.toLower());
    dir.cd(siteName.toLower());
    dir.mkdir("pages");
    dir.mkdir("posts");
    dir.mkdir("includes");
    dir.mkdir("layouts");
    dir.mkdir("assets");
    dir.cd("assets");
    dir.mkdir("css");
    dir.mkdir("fonts");
    dir.mkdir("js");
    dir.mkdir("images");

    QFile file(path + "/Site.xml");
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/Site.xml";
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.writeStartDocument();
    xml.writeStartElement("Site");
    xml.writeAttribute("theme", field("theme").toString());
    xml.writeAttribute("title", siteName);
    if(!description.isEmpty())
        xml.writeAttribute("description", description);
    if(!copyright.isEmpty())
        xml.writeAttribute("copyright", copyright);

    xml.writeStartElement("Content");
    xml.writeAttribute("type", "page");
    xml.writeAttribute("source", "index.xml");
    xml.writeAttribute("title", "Index");
    xml.writeAttribute("menu", "default");
    xml.writeAttribute("author", "admin");
    xml.writeAttribute("layout", "default");
    xml.writeAttribute("date", QString(QDate::currentDate().toString("dd.MM.yyyy")));

    xml.writeStartElement("Menu");
    xml.writeAttribute("name", "default");

    xml.writeStartElement("Item");
    xml.writeAttribute("title", "Index");
    xml.writeAttribute("url", "index.html");

    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

    QFile index(path + "/pages/index.xml");
    if(!index.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/pages/index.xml";
        return;
    }
    QXmlStreamWriter ind(&index);
    ind.writeStartDocument();
    ind.writeStartElement("Content");
    ind.writeStartElement("Section");
    ind.writeStartElement("Row");
    ind.writeStartElement("Column");
    ind.writeAttribute("span", "12");
    ind.writeStartElement("Text");
    ind.writeCDATA("<h1>Welcome</h1>");
    ind.writeEndElement();
    ind.writeEndElement();
    ind.writeEndElement();
    ind.writeEndElement();
    ind.writeEndElement();
    ind.writeEndDocument();
    index.close();

    QDialog::accept();

    emit loadSite(path + "/Site.xml");
    emit buildSite();
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel(tr("This wizard will generate a skeleton website. "
                          "You simply need to specify the site name and set a "
                          "few options to produce the site."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

SiteInfoPage::SiteInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Site Information"));
    setSubTitle(tr("Specify basic information about the site for which you "
                   "want to generate site files."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/icon64.png"));

    m_siteNameLabel = new QLabel("&Site title:");
    m_siteNameLineEdit = new QLineEdit;
    m_siteNameLabel->setBuddy(m_siteNameLineEdit);
    m_siteNameLineEdit->setPlaceholderText("Site title");

    m_descriptionLabel = new QLabel("&Description:");
    m_descriptionLineEdit = new QLineEdit;
    m_descriptionLabel->setBuddy(m_descriptionLineEdit);
    m_descriptionLineEdit->setPlaceholderText("Site description");

    m_copyrightLabel = new QLabel("&Copyright");
    m_copyrightLineEdit = new QLineEdit;
    m_copyrightLabel->setBuddy(m_copyrightLineEdit);
    m_copyrightLineEdit->setPlaceholderText("&copy; 2017 My Company. All Rights Reserved.");

    m_themeLabel = new QLabel("&Theme");
    m_theme = new QComboBox;
    m_themeLabel->setBuddy(m_theme);
    m_theme->addItem("default");
    m_theme->addItem("himu");

    registerField("siteName*", m_siteNameLineEdit);
    registerField("description", m_descriptionLineEdit);
    registerField("copyright", m_copyrightLineEdit);
    registerField("theme", m_theme, "currentText");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_siteNameLabel, 0, 0);
    layout->addWidget(m_siteNameLineEdit, 0, 1);
    layout->addWidget(m_descriptionLabel, 1, 0);
    layout->addWidget(m_descriptionLineEdit, 1, 1);
    layout->addWidget(m_copyrightLabel, 2, 0);
    layout->addWidget(m_copyrightLineEdit, 2, 1);
    layout->addWidget(m_themeLabel, 3, 0);
    layout->addWidget(m_theme, 3, 1);
    setLayout(layout);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the site skeleton.").arg(finishText));
}
