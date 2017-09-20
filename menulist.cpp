#include "menulist.h"
#include "mainwindow.h"
#include "menueditor.h"
#include "commands.h"
#include "flatbutton.h"
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

    m_undo = new FlatButton(":/images/undo_normal.png", ":/images/undo_hover.png", "", ":/images/undo_disabled.png");
    m_redo = new FlatButton(":/images/redo_normal.png", ":/images/redo_hover.png", "", ":/images/redo_disabled.png");
    m_undo->setToolTip("Undo");
    m_redo->setToolTip("Redo");
    m_undo->setEnabled(false);
    m_redo->setEnabled(false);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch(0);
    hbox->addWidget(m_undo);
    hbox->addWidget(m_redo);

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
    layout->addLayout(hbox, 0, 2);
    layout->addWidget(button, 1, 0);
    layout->addWidget(m_list, 2, 0, 1, 3);
    setLayout(layout);

    reloadMenu();

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
    connect(m_redo, SIGNAL(clicked()), this, SLOT(redo()));
    connect(m_undo, SIGNAL(clicked()), this, SLOT(undo()));
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SLOT(canUndoChanged(bool)));
    connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SLOT(canRedoChanged(bool)));
    connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SLOT(undoTextChanged(QString)));
    connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SLOT(redoTextChanged(QString)));
}

MenuList::~MenuList()
{
    delete m_undoStack;
}

void MenuList::canUndoChanged(bool can)
{
    m_undo->setEnabled(can);
}

void MenuList::canRedoChanged(bool can)
{
    m_redo->setEnabled(can);
}

void MenuList::undoTextChanged(QString text)
{
    m_undo->setToolTip("Undo " + text);
}

void MenuList::redoTextChanged(QString text)
{
    m_redo->setToolTip("Redo " + text);
}

void MenuList::undo()
{
    m_undoStack->undo();
}

void MenuList::redo()
{
    m_undoStack->redo();
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
    m_site->reloadMenus();
    m_menuInEditor = NULL;

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

    if(m_editor)
        m_editor->reloadMenu(m_menuInEditor);
}

void MenuList::saveMenu()
{
    m_site->saveMenus();
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
    menuChanged("menu added");
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
            m_site->removeMenu(m);
            m_list->removeRow(row);
            menuChanged("menu \"" + m->name() + "\" deleted");
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
