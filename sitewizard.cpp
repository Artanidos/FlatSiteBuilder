#include "sitewizard.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>
#include <QDomDocument>
#include <QTest>

SiteWizard::SiteWizard()
{
    addPage(new IntroPage);
    addPage(new SiteInfoPage);
    addPage(new ConclusionPage);

    //setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    //setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Site Wizard"));
}

void SiteWizard::accept()
{
    QString siteName = field("siteName").toString();
    QString description = field("description").toString();
    QString copyright = field("copyright").toString();
    QString githubUrl = field("githubUrl").toString();
    QString path = QDir::homePath() + "/FlatSiteBuilder/sources/" + siteName.toLower();

    QDir dir(QDir::homePath() + "/FlatSiteBuilder/sources/");
    dir.mkdir(siteName.toLower());
    dir.cd(siteName.toLower());
    dir.mkdir("pages");
    dir.mkdir("posts");
    dir.mkdir("includes");
    dir.mkdir("assets");
    dir.cd("assets");
    dir.mkdir("css");
    dir.mkdir("images");

    QDomDocument doc;
    QDomElement root;
    QFile file(path + "/Site.xml");
    if(!file.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/Site.xml";
        return;
    }
    root = doc.createElement("Site");
    root.setAttribute("theme", field("theme").toString());
    root.setAttribute("title", siteName);
    if(!description.isEmpty())
        root.setAttribute("description", description);
    if(!githubUrl.isEmpty())
        root.setAttribute("github", githubUrl);
    if(!copyright.isEmpty())
        root.setAttribute("copyright", copyright);
    doc.appendChild(root);


    QDomElement c = doc.createElement("Content");
    c.setAttribute("type", "page");
    c.setAttribute("source", "index.xml");
    c.setAttribute("title", "Index");
    c.setAttribute("menu", "default");
    c.setAttribute("author", "admin");
    c.setAttribute("layout", "default");
    //if(content->contentType() == ContentType::Post)
    //    c.setAttribute("excerpt", content->excerpt());
    c.setAttribute("date", QString(QDate::currentDate().toString("dd.MM.yyyy")));
    root.appendChild(c);


    QDomElement m = doc.createElement("Menu");
    m.setAttribute("name", "default");

    QDomElement i = doc.createElement("Item");
    i.setAttribute("title", "Index");
    i.setAttribute("url", "index.html");
    m.appendChild(i);

    root.appendChild(m);

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    QFile index(path + "/pages/index.xml");
    if(!index.open(QFile::WriteOnly))
    {
        qDebug() << "Unable to open file " + path + "/pages/index.xml";
        return;
    }
    QDomDocument idoc;
    QDomElement content = idoc.createElement("Content");
    QDomElement section = idoc.createElement("Section");
    QDomElement row = idoc.createElement("Row");
    QDomElement column = idoc.createElement("Column");
    QDomElement text = idoc.createElement("Text");
    text.appendChild(idoc.createCDATASection("<h1>Welcome</h1>"));
    column.appendChild(text);
    row.appendChild(column);
    section.appendChild(row);
    content.appendChild(section);
    idoc.appendChild(content);

    QTextStream istream(&index);
    istream << idoc.toString();
    index.close();

    QDialog::accept();

    emit loadSite(path + "/Site.xml");
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

    m_githubUrlLabel = new QLabel("&Github Url");
    m_githubUrlLineEdit = new QLineEdit;
    m_githubUrlLabel->setBuddy(m_githubUrlLineEdit);
    m_githubUrlLineEdit->setPlaceholderText("https://github.com/mycompany/myproject.git");

    m_themeLabel = new QLabel("&Theme");
    m_theme = new QComboBox;
    m_themeLabel->setBuddy(m_theme);
    m_theme->addItem("default");
    m_theme->setEnabled(false);

    registerField("siteName*", m_siteNameLineEdit);
    registerField("description", m_descriptionLineEdit);
    registerField("copyright", m_copyrightLineEdit);
    registerField("githubUrl", m_githubUrlLineEdit);
    registerField("theme", m_theme, "currentText");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(m_siteNameLabel, 0, 0);
    layout->addWidget(m_siteNameLineEdit, 0, 1);
    layout->addWidget(m_descriptionLabel, 1, 0);
    layout->addWidget(m_descriptionLineEdit, 1, 1);
    layout->addWidget(m_copyrightLabel, 2, 0);
    layout->addWidget(m_copyrightLineEdit, 2, 1);
    layout->addWidget(m_githubUrlLabel, 3, 0);
    layout->addWidget(m_githubUrlLineEdit, 3, 1);
    layout->addWidget(m_themeLabel, 4, 0);
    layout->addWidget(m_theme, 4, 1);
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