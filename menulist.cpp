#include "menulist.h"
#include "mainwindow.h"
#include "menueditor.h"
#include "commands.h"
#include <QStatusBar>
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include <QUndoCommand>
#include <QUndoStack>
#include <QXmlStreamWriter>
#include <QTest>
#include "flatbutton.h"
#include "tablecellbuttons.h"

MenuList::MenuList(MainWindow *win, Site *site)
{
    m_win = win;
    m_site = site;
    m_menuInEditor = NULL;
    m_editor = NULL;
    m_undoStack = new QUndoStack;
    QPushButton *button = new QPushButton("Add Menu");
    button->setMaximumWidth(120);
    QLabel *titleLabel = new QLabel("Menus");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    m_list = new QTableWidget(0, 2, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name";
    m_list->setHorizontalHeaderLabels(labels);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(titleLabel, 0, 0);
    layout->addWidget(button, 1, 0);
    layout->addWidget(m_list, 2, 0, 1, 3);
    setLayout(layout);

    reloadMenu();

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
}

MenuList::~MenuList()
{
    delete m_undoStack;
}

void MenuList::registerMenuEditor(MenuEditor *editor)
{
    m_editor = editor;
    m_editor->registerUndoStack(m_undoStack);
    connect(m_editor, SIGNAL(menuChanged(QString)), this, SLOT(menuChanged(QString)));
}

void MenuList::unregisterMenuEditor()
{
    m_editor = NULL;
}

void MenuList::reloadMenu()
{
    m_list->clearContents();
    m_list->setRowCount(0);
    m_site->removeAllMenus();

    QFile file(m_site->sourcePath() + "/Menus.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        m_win->statusBar()->showMessage("Unable to open " + m_site->sourcePath() + "/Menus.xml");
        return;
    }
    int id = 0;
    QXmlStreamReader xml(&file);
    if(xml.readNextStartElement())
    {
        if(xml.name() == "Menus")
        {
            while(xml.readNextStartElement())
            {
                if(xml.name() == "Menu")
                {
                    Menu *m = new Menu();
                    m->setId(id++);
                    m->setName(xml.attributes().value("name").toString());
                    while(xml.readNextStartElement())
                    {
                        if(xml.name() == "Item")
                        {
                            MenuItem *item = new MenuItem();
                            item->setTitle(xml.attributes().value("title").toString());
                            item->setUrl(xml.attributes().value("url").toString());
                            item->setIcon(xml.attributes().value("icon").toString());

                            while(xml.readNextStartElement())
                            {
                                if(xml.name() == "Item")
                                {
                                    MenuItem *subitem = new MenuItem();
                                    subitem->setSubitem(true);
                                    subitem->setTitle(xml.attributes().value("title").toString());
                                    subitem->setUrl(xml.attributes().value("url").toString());
                                    subitem->setIcon(xml.attributes().value("icon").toString());
                                    item->addMenuitem(subitem);
                                    xml.readNext();
                                }
                                else
                                    xml.skipCurrentElement();

                            }
                            m->addMenuitem(item);
                            xml.readNext();
                        }
                        else
                            xml.skipCurrentElement();
                    }
                    m_site->addMenu(m);
                }
                else
                    xml.skipCurrentElement();
            }
        }
    }
    file.close();

    foreach(Menu *menu, m_site->menus())
    {
        QTableWidgetItem *item = addListItem(menu);
        if(m_menuInEditor)
        {
            if(menu->id() == m_menuInEditor->id())
            {
                m_list->selectRow(m_list->rowCount() - 1);
                m_menuInEditor = menu;
                emit editedItemChanged(item);
            }
        }
    }

    m_win->statusBar()->showMessage("Menus have been loaded");
    if(m_editor)
        m_editor->reloadMenu(m_menuInEditor);
}

void MenuList::saveMenu()
{
    QFile file(m_site->sourcePath() + "/Menus.xml");
    if(!file.open(QFile::WriteOnly))
    {
        m_win->statusBar()->showMessage("Unable to open file " + m_site->sourcePath() + "/Menus.xml");
        return;
    }
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeStartElement("Menus");
    foreach(Menu *menu, m_site->menus())
    {
        xml.writeStartElement("Menu");
        xml.writeAttribute("name", menu->name());
        foreach(MenuItem *item, menu->items())
        {
            xml.writeStartElement("Item");
            xml.writeAttribute("title", item->title());
            xml.writeAttribute("url", item->url());
            xml.writeAttribute("icon", item->icon());
            foreach(MenuItem *subitem, item->items())
            {
                xml.writeStartElement("Item");
                xml.writeAttribute("title", subitem->title());
                xml.writeAttribute("url", subitem->url());
                xml.writeAttribute("icon", subitem->icon());
                xml.writeEndElement();
            }
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();

    m_win->statusBar()->showMessage("Menus have been saved");
}

void MenuList::menuChanged(QString text)
{
    QUndoCommand *changeCommand = new ChangeMenuCommand(this, m_site, text);
    m_undoStack->push(changeCommand);
    m_win->statusBar()->showMessage("Menu saved. The project should be rebuildet on the dasboard later to affect changes.");
}

QTableWidgetItem *MenuList::addListItem(Menu *menu)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCellButtons *tcb = new TableCellButtons;
    tcb->setItem(menu);
    connect(tcb, SIGNAL(deleteItem(QObject*)), this, SLOT(deleteMenu(QObject*)));
    connect(tcb, SIGNAL(editItem(QObject*)), this, SLOT(editMenu(QObject*)));
    m_list->setCellWidget(rows, 0, tcb);
    m_list->setRowHeight(rows, tcb->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(menu->name());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(menu));
    m_list->setItem(rows, 1, titleItem);
    return titleItem;
}

void MenuList::buttonClicked()
{
    Menu *menu = new Menu();
    m_site->addMenu(menu);
    addListItem(menu);
    emit contentUpdated("Menu added");
    tableDoubleClicked(m_list->rowCount() - 1, 0);
}

void MenuList::deleteMenu(QObject *menu)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Menu *m = qvariant_cast<Menu*>(item->data(Qt::UserRole));
        if(m == menu)
        {
            m_site->removeMenu(dynamic_cast<Menu*>(menu));
            m_list->removeRow(row);
            emit contentUpdated("Menu deleted");
            break;
        }
    }
}

void MenuList::editMenu(QObject *menu)
{
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        QTableWidgetItem *item = m_list->item(row, 1);
        Menu *m = qvariant_cast<Menu*>(item->data(Qt::UserRole));
        if(m == menu)
        {
            m_menuInEditor = m;
            m_list->selectRow(row);
            emit editContent(item);
            break;
        }
    }
}

void MenuList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    m_menuInEditor = qvariant_cast<Menu*>(item->data(Qt::UserRole));
    emit editContent(item);
}
