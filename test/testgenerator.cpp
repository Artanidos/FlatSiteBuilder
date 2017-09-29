#include <QtTest/QtTest>
#include <QObject>
#include "../generator.h"
#include "../plugins.h"

class TestGenerator : public QObject
{
    Q_OBJECT
private slots:
    void generateSite();
    void translateContent_data();
    void translateContent();
    void nextTokens_data();
    void nextTokens();
    void initTestCase();
    void cleanupTestCase();
};

class TextEditor : public ElementEditorInterface
{
public:
    TextEditor() {}
    void setContent(QString) {}
    QString className() override {return "TextEditor";}
    QString displayName() override {return "Text";}
    QString tagName() override {return "Text";}
    QImage icon() override {return QImage(":/images/text.png");}
    QString getHtml(QXmlStreamReader *) {return "<h1>testPageTitle</h1>";}
    QString load(QXmlStreamReader *) override {return "";}
};

void TestGenerator::initTestCase()
{
    QDir dir("/home/olaf/SourceCode/FlatSiteBuilder/test");
    dir.mkdir("sites");
}

void TestGenerator::cleanupTestCase()
{
    QDir dir("/home/olaf/SourceCode/FlatSiteBuilder/test/sites");
    dir.removeRecursively();
}

void TestGenerator::nextTokens_data()
{
    QTest::addColumn<QString>("content");
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("result");

    QTest::newRow("endif a") << "{% endif %}" << (QStringList() << "{%" << "endif" << "%}") << true;
    QTest::newRow("endif b") << "{%  endif %}" << (QStringList() << "{%" << "endif" << "%}") << true;
    QTest::newRow("endif c") << "{% endif  %}" << (QStringList() << "{%" << "endif" << "%}") << true;
    QTest::newRow("endif d") << "{% endifa  %}" << (QStringList() << "{%" << "endif" << "%}") << false;
    QTest::newRow("endif e") << "{% aendif  %}" << (QStringList() << "{%" << "endif" << "%}") << false;
    QTest::newRow("endif f") << "{%a endif  %}" << (QStringList() << "{%" << "endif" << "%}") << false;
    QTest::newRow("endif g") << "{% endif  a%}" << (QStringList() << "{%" << "endif" << "%}") << false;
    QTest::newRow("endif h") << "a{% endif  %}" << (QStringList() << "{%" << "endif" << "%}") << false;
    QTest::newRow("endif i") << "{% endif  %}a" << (QStringList() << "{%" << "endif" << "%}") << true;
}

void TestGenerator::nextTokens()
{
    QFETCH(QString, content);
    QFETCH(QStringList, tokens);
    QFETCH(bool, result);

    Generator g;
    QCOMPARE(g.nextTokens(content, tokens), result);
}

void TestGenerator::translateContent_data()
{
    QTest::addColumn<QString>("var");
    QTest::addColumn<QString>("result");

    QTest::newRow("page") << "{{ page.title }}" << "PageTitle";
    QTest::newRow("site") << "{{ site.title }}" << "SiteTitle";
    QTest::newRow("loop") << "{{ item.title }}" << "ItemTitle";
    QTest::newRow("wrong loop") << "{{ ite.title }}" << "";

    QTest::newRow("loop without spaces") << "{{item.title}}" << "ItemTitle";
    QTest::newRow("loop left space") << "{{ item.title}}" << "ItemTitle";
    QTest::newRow("loop left spaces") << "{{  item.title}}" << "ItemTitle";
    QTest::newRow("loop right space") << "{{item.title }}" << "ItemTitle";
    QTest::newRow("loop right spaces") << "{{item.title  }}" << "ItemTitle";

    QTest::newRow("for a") << "{% for item in site.menus[page.menu] %}{{ item.title }}{% endfor %}" << "MenuItemTitle";
    QTest::newRow("for b") << "{% for item in site.menus[page.menu] %}{{ item.title }}{%endfor %}" << "MenuItemTitle";
    QTest::newRow("for c") << "{% for item in site.menus[page.menu] %}{{ item.title }}{% endfor%}" << "MenuItemTitle";
    QTest::newRow("for d") << "{% for item in site.menus[page.menu] %}{{ item.title }}{%endfor%}" << "MenuItemTitle";
    QTest::newRow("for e") << "{% for item in site.menus[page.menu] %}\n{{ item.title }}\n{%endfor%}\n" << "MenuItemTitle\n";
    QTest::newRow("for wrong index") << "{% for item in site.menus[page.menus] %}{{ item.title }}{% endfor %}" << "";

    QTest::newRow("for nested a") << "{% for item in site.menus[page.menu] %}{% for subitem in item.items %}{{ subitem.title }}{% endfor %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested b") << "{% for item in site.menus[page.menu] %}{%  for subitem in item.items %}{{ subitem.title }}{% endfor %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested c") << "{% for item in site.menus[page.menu] %}{% for subitem  in item.items %}{{ subitem.title }}{% endfor %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested d") << "{% for item in site.menus[page.menu] %}{% for subitem in  item.items %}{{ subitem.title }}{% endfor %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested e") << "{% for item in site.menus[page.menu] %}{% for subitem in item.items %}{{ subitem.title }}{%  endfor %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested f") << "{% for item in site.menus[page.menu] %}{% for subitem in item.items %}{{ subitem.title }}{% endfor  %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested g") << "{% for item in site.menus[page.menu] %}{% for subitem in item.items %}{{ subitem.title }}{%endfor %}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested h") << "{% for item in site.menus[page.menu] %}{% for subitem in item.items %}{{ subitem.title }}{% endfor%}{% endfor %}" << "SubItemTitle";
    QTest::newRow("for nested i") << "{% for item in site.menus[page.menu] %}{% for subitem in item.items %}{{ subitem.title }}{%endfor%}{% endfor %}" << "SubItemTitle";

    QTest::newRow("if equal theme a") << "{% if theme.switch %}left{% endif %}" << "left";
    QTest::newRow("if equal theme b") << "{% if theme.switch == true %}left{% endif %}" << "left";
    QTest::newRow("if equal theme c") << "{% if theme.switch == false %}left{% endif %}" << "";
    QTest::newRow("if equal theme d") << "{% if theme.noswitch %}left{% endif %}" << "";
    QTest::newRow("if equal theme e") << "{% if theme.noswitch == true %}left{% endif %}" << "";
    QTest::newRow("if equal theme f") << "{% if theme.noswitch == false %}left{% endif %}" << "left";

    QTest::newRow("if equal theme undef a") << "{% if theme.switc %}left{% endif %}" << "";
    QTest::newRow("if equal theme undef b") << "{% if theme.switc == true %}left{% endif %}" << "";

    QTest::newRow("if not equal theme a") << "{% if theme.switch != true %}left{% endif %}" << "";
    QTest::newRow("if not equal theme c") << "{% if theme.switch != false %}left{% endif %}" << "left";
    QTest::newRow("if not equal theme d") << "{% if theme.noswitch != true %}left{% endif %}" << "left";
    QTest::newRow("if not equal theme e") << "{% if theme.noswitch != false %}left{% endif %}" << "";

    QTest::newRow("if not equal theme undef a") << "{% if theme.switc != true %}left{% endif %}" << "left";
    QTest::newRow("if not equal theme undef b") << "{% if theme.switc == false %}left{% endif %}" << "left";

    QTest::newRow("if equal a") << "{% if site.title == \"SiteTitle\" %}true{% endif %}" << "true";
    QTest::newRow("if equal b") << "{% if site.title == \"SiteTitl\" %}true{% endif %}" << "";
    QTest::newRow("if not equal") << "{% if site.title != \"Siteitle\" %}false{% endif %}" << "false";
    QTest::newRow("if else a") << "{% if site.title == \"Siteitle\" %}true{% else %}false{% endif %}" << "false";
    QTest::newRow("if else b") << "{% if site.title == \"SiteTitle\" %}true{% else %}false{% endif %}" << "true";
    QTest::newRow("if equal space a") << "{% if  site.title == \"SiteTitle\" %}true{% endif %}" << "true";
    QTest::newRow("if equal space b") << "{% if site.title  == \"SiteTitle\" %}true{% endif %}" << "true";
    QTest::newRow("if equal space c") << "{% if site.title ==  \"SiteTitle\" %}true{% endif %}" << "true";
    QTest::newRow("if equal space d") << "{% if site.title == \"SiteTitle\"  %}true{% endif %}" << "true";
    QTest::newRow("if equal space e") << "{% if site.title == \"SiteTitle\" %}true{%  endif %}" << "true";
    QTest::newRow("if equal space f") << "{% if site.title == \"SiteTitle\" %}true{% endif  %}" << "true";
    QTest::newRow("if else space a") << "{% if site.title == \"SiteTitle\" %}true{%  else %}false{% endif %}" << "true";
    QTest::newRow("if else space b") << "{% if site.title == \"SiteTitle\" %}true{% else  %}false{% endif %}" << "true";
    QTest::newRow("if else no space a") << "{% if site.title == \"SiteTitle\" %}true{%else%}false{% endif %}" << "true";
    QTest::newRow("if else no space b") << "{% if site.title == \"SiteTitle\" %}true{%else %}false{% endif %}" << "true";
    QTest::newRow("if else no space c") << "{% if site.title == \"SiteTitle\" %}true{% else%}false{% endif %}" << "true";
    QTest::newRow("if endif no space a") << "{% if site.title == \"SiteTitle\" %}true{% else %}false{%endif%}" << "true";
    QTest::newRow("if endif no space b") << "{% if site.title == \"SiteTitle\" %}true{% else %}false{%endif %}" << "true";
    QTest::newRow("if endif no space c") << "{% if site.title == \"SiteTitle\" %}true{% else%}false{% endif%}" << "true";

    QTest::newRow("if nested a") << "{% if site.title == \"SiteTitle\" %}{% if page.title == site.title %}A{% else %}B{% endif %}{% else %}C{% endif %}" << "B";
    QTest::newRow("if nested b") << "{% if site.title == \"SiteTitle\" %}A{% else %}{% if page.title == site.title %}B{% else %}C{% endif %}{% endif %}" << "A";
    QTest::newRow("if nested c") << "{% if site.title == \"Sitetitle\" %}{% if page.title == site.title %}A{% else %}B{% endif %}{% else %}C{% endif %}" << "C";
    QTest::newRow("if nested d") << "{% if site.title == \"Sitetitle\" %}{% if page.title == site.title %}A{% else %}B{%  endif %}{% else %}C{% endif %}" << "C";
    QTest::newRow("if nested e") << "{% if site.title == \"SiteTitle\" %}A{% else %}{% if page.title == site.title %}B{% else %}C{%  endif %}{% endif %}D" << "AD";
}

void TestGenerator::translateContent()
{
    QFETCH(QString, var);
    QFETCH(QString, result);

    QVariantMap loopvars;
    Generator g;
    QVariantMap cm;
    cm["title"] = "ItemTitle";
    loopvars["item"] = cm;

    QVariantMap menus;
    QVariantList items;
    QVariantMap menuitem;
    menuitem["title"] = "MenuItemTitle";
    QVariantList subitems;
    QVariantMap submenuitem;
    submenuitem["title"] = "SubItemTitle";
    subitems.append(submenuitem);
    menuitem["items"] = subitems;
    menuitem["hasItems"] = "true";
    items.append(menuitem);
    menus["MenuName"] = items;

    g.addSiteVar("menus", menus);
    g.addSiteVar("title", "SiteTitle");
    g.addThemeVar("switch", "true");
    g.addThemeVar("noswitch", "false");
    g.addPageVar("title", "PageTitle");
    g.addPageVar("menu", "MenuName");
    QString rc = g.translateContent(var, loopvars);
    QCOMPARE(rc, result);
}

void TestGenerator::generateSite()
{
    Plugins::insert("TextEditor", new TextEditor());
    QString dir = "/home/olaf/SourceCode/FlatSiteBuilder/test";
    Generator g;
    g.setSitesPath(dir + "/sites");
    g.setThemePath(dir + "/themes");

    Site *site = new Site(NULL, dir + "/Site.xml");
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
    site->addPage(page);
    g.generateSite(NULL, site);

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

QTEST_MAIN(TestGenerator)
#include "testgenerator.moc"
