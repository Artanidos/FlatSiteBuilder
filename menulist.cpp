#include "menulist.h"
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include "flatbutton.h"
#include "tablecellbuttons.h"

MenuList::MenuList(Site *site)
{
    m_site = site;
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

    if(m_site)
    {
        for(int i = 0; i < m_site->menus().count(); i++)
        {
            Menu *menu = m_site->menus().at(i);
            addListItem(menu);
        }
    }

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
}

void MenuList::addListItem(Menu *menu)
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
            m_list->selectRow(row);
            emit editContent(item);
            break;
        }
    }
}

void MenuList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    emit editContent(item);
}
