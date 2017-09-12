#include "menulist.h"
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include "tablecheckbox.h"

MenuList::MenuList(Site *site)
{
    m_site = site;
    QPushButton *button = new QPushButton("Add Menu");
    button->setMaximumWidth(120);
    m_deleteButton = new QPushButton("Delete");
    m_deleteButton->setMaximumWidth(120);
    m_deleteButton->setEnabled(false);
    m_deleteButton->setToolTip("Delete all marked items");
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
    layout->addWidget(m_deleteButton, 3, 0);
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
    connect(m_deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
}

void MenuList::addListItem(Menu *menu)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCheckbox *checkBox = new TableCheckbox();
    connect(checkBox, SIGNAL(checkStateChanged(bool)), this, SLOT(checkStateChanged(bool)));
    m_list->setCellWidget(rows, 0, checkBox);
    m_list->setRowHeight(rows, checkBox->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(menu->name());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(menu));
    m_list->setItem(rows, 1, titleItem);
}

void MenuList::checkStateChanged(bool)
{
    int numberChecked = 0;
    for(int row = 0; row < m_list->rowCount(); row++)
    {
        TableCheckbox *cb = dynamic_cast<TableCheckbox*>(m_list->cellWidget(row, 0));
        if(cb->checked() == Qt::Checked)
        {
            numberChecked++;
            break;
        }
    }
    m_deleteButton->setEnabled(numberChecked > 0);
}

void MenuList::buttonClicked()
{
    Menu *menu = new Menu();
    m_site->addMenu(menu);
    addListItem(menu);
    emit contentUpdated("Menu added");
    tableDoubleClicked(m_list->rowCount() - 1, 0);
}

void MenuList::deleteButtonClicked()
{
    for(int row = m_list->rowCount() - 1; row >= 0; row--)
    {
        TableCheckbox *cb = dynamic_cast<TableCheckbox*>(m_list->cellWidget(row, 0));
        if(cb->checked() == Qt::Checked)
        {
            QTableWidgetItem *item = m_list->item(row, 1);
            Menu *menu = qvariant_cast<Menu*>(item->data(Qt::UserRole));
            m_site->removeMenu(menu);
            m_list->removeRow(row);
        }
    }
    m_deleteButton->setEnabled(false);
    emit contentUpdated("Menu deleted");
}

void MenuList::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    emit editContent(item);
}
