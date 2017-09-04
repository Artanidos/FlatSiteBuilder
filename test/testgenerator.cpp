
#include <QtTest/QtTest>
#include <QObject>
#include "../generator.h"

class TestGenerator : public QObject
{
    Q_OBJECT
private slots:
    //void generateSite();
    //void translateContent();
    void initTestCase();
    void cleanupTestCase();
};

void TestGenerator::initTestCase()
{
    QDir dir(QDir::currentPath());
    dir.mkdir("sites");
}

void TestGenerator::cleanupTestCase()
{
    QDir dir(QDir::currentPath() + "/sites");
    dir.removeRecursively();
}

/*
void TestGenerator::translateContent()
{
    QVariantMap loopvars;
    Generator g;
    QString rc = g.translateContent("{{ site.title }}", loopvars);
    QCOMPARE(rc, "TestTitle");
}
*/
/*
void TestGenerator::generateSite()
{
    QMap<QString, QObject*> plugins;
    plugins.insert("SectionPropertyEditor", new SectionPropertyEditor());
    plugins.insert("RowPropertyEditor", new RowPropertyEditor());
    //plugins.insert("TextEditor", new TextEditor());
    QString dir = QDir::currentPath();
    Generator g;
    g.setSitesPath(dir + "/sites");
    g.setThemePath(dir + "/themes");

    Site *site = new Site(dir + "/Site.xml");
    site->setCopyright("siteCopyright");
    site->setTheme("testSiteTheme");
    site->setTitle("testSiteTitle");
    site->setAuthor("testSiteAuthor");
    site->setDescription("testSiteDescription");
    site->setKeywords("testSiteKeywords");

    Content *page = new Content(ContentType::Page);
    page->setSource("test.xml");
    page->setTitle("testPageTitle");
    page->setLayout("testPageLayout");
    page->setMenu("testMenu");

    Menu *menu = new Menu();
    menu->setName("testMenu");
    MenuItem *item = new MenuItem();
    item->setTitle("testMenuItemTitle");
    item->setUrl("test.html");
    menu->addMenuitem(item);
    MenuItem *item2 = new MenuItem();
    item2->setTitle("testMenuItemTitle2");
    item2->setUrl("testMenuItemUrl2");
    menu->addMenuitem(item2);
    site->addMenu(menu);
    site->addContent(page);
    g.generateSite(site, plugins);

    QStringList list;
    list << "<html>\n";
    list << "<head>\n";
    list << "<meta name=\"description\" content=\"testSiteDescription\">\n";
    list << "<meta name=\"keywords\" content=\"testSiteKeywords\"/>\n";
    list << "<meta name=\"author\" content=\"testSiteAuthor\">\n";
    list << "<title>testSiteTitle</title>\n";
    list << "</head>\n";
    list << "<body>\n";
    list << "<ul>\n";
    list << "<li class=\"active\"><a href=\"test.html\">testMenuItemTitle</a></li>\n";
    list << "<li><a href=\"testMenuItemUrl2\">testMenuItemTitle2</a></li>\n";
    list << "</ul>\n";
    list << "<section class=\"container\">\n";
    list << "<div class=\"row\">\n";
    list << "<div class=\"col-md-12\">\n";
    list << "<h1>testPageTitle</h1>\n";
    list << "</div>\n";
    list << "</div>\n";
    list << "</section>\n";
    list << "</body>\n";
    list << "</html>\n";
    QFile file(dir + "/sites/testSiteTitle/test.html");
    QVERIFY2(file.exists(), "Output file has not been generated");
    if(file.open(QFile::ReadOnly))
    {
        for(int i=0; i < list.count(); i++)
        {
            QString content = QString::fromLatin1(file.readLine());
            QCOMPARE(content, list.at(i));
        }
        file.close();
    }
}
*/

QTEST_MAIN(TestGenerator)
#include "testgenerator.moc"
