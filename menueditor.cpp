#include "menueditor.h"
#include "flatbutton.h"
#include "tablecheckbox.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>

MenuEditor::MenuEditor(Menu *menu)
{
    m_menu = menu;
    m_changed = false;
    setAutoFillBackground(true);

    QLabel *titleLabel = new QLabel("Menu Editor");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(20);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    m_close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    m_close->setToolTip("Close Content Editor");
    m_undo = new FlatButton(":/images/undo_normal.png", ":/images/undo_hover.png", "", ":/images/undo_disabled.png");
    m_redo = new FlatButton(":/images/redo_normal.png", ":/images/redo_hover.png", "", ":/images/redo_disabled.png");
    m_undo->setToolTip("Undo");
    m_redo->setToolTip("Redo");
    m_undo->setEnabled(false);
    m_redo->setEnabled(false);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(m_undo);
    hbox->addWidget(m_redo);
    hbox->addWidget(m_close);

    QPushButton *button = new QPushButton("Add Menuitem");
    button->setMaximumWidth(120);

    m_name = new QLineEdit;
    m_name->setText(menu->name());

    m_list = new QTableWidget(0, 4, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Title" << "Url" << "Icon";
    m_list->setHorizontalHeaderLabels(labels);

    m_deleteButton = new QPushButton("Delete");
    m_deleteButton->setMaximumWidth(120);
    m_deleteButton->setEnabled(false);
    m_deleteButton->setToolTip("Delete all marked items");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(titleLabel, 0, 0);
    layout->addLayout(hbox, 0, 2);
    layout->addWidget(new QLabel("Name"), 1, 0);
    layout->addWidget(m_name, 2, 0);
    layout->addWidget(button, 3, 0);
    layout->addWidget(m_list, 4, 0, 1, 3);
    layout->addWidget(m_deleteButton, 5, 0);
    setLayout(layout);

    for(int i = 0; i < m_menu->items().count(); i++)
    {
        MenuItem *item = m_menu->items().at(i);
        addListItem(item);
    }

    connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
    connect(m_close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_name, SIGNAL(editingFinished()), this, SLOT(nameChanged()));
    connect(m_deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
}

void MenuEditor::addListItem(MenuItem *item)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCheckbox *checkBox = new TableCheckbox();
    connect(checkBox, SIGNAL(checkStateChanged(bool)), this, SLOT(checkStateChanged(bool)));
    m_list->setCellWidget(rows, 0, checkBox);
    m_list->setRowHeight(rows, checkBox->sizeHint().height());

    QTableWidgetItem *titleItem = new QTableWidgetItem(item->title());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(item));
    m_list->setItem(rows, 1, titleItem);

    QTableWidgetItem *urlItem = new QTableWidgetItem(item->url());
    urlItem->setFlags(urlItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 2, urlItem);

    QTableWidgetItem *iconItem = new QTableWidgetItem(item->icon());
    iconItem->setFlags(iconItem->flags() ^ Qt::ItemIsEditable);
    m_list->setItem(rows, 3, iconItem);
}

void MenuEditor::closeEditor()
{
    emit close();
}

void MenuEditor::nameChanged()
{
    if(m_menu->name() != m_name->text())
    {
        m_menu->setName(m_name->text());
        emit contentChanged(m_menu);
        emit contentUpdated("Titel Changed");
    }
}

void MenuEditor::checkStateChanged(bool)
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

void MenuEditor::buttonClicked()
{
    MenuItem *item = new MenuItem();
    m_menu->addMenuitem(item);
    addListItem(item);
    emit contentUpdated("Menuitem added");
    tableDoubleClicked(m_list->rowCount() - 1, 0);
}

void MenuEditor::deleteButtonClicked()
{
    for(int row = m_list->rowCount() - 1; row >= 0; row--)
    {
        TableCheckbox *cb = dynamic_cast<TableCheckbox*>(m_list->cellWidget(row, 0));
        if(cb->checked() == Qt::Checked)
        {
            QTableWidgetItem *item = m_list->item(row, 1);
            MenuItem *i = qvariant_cast<MenuItem*>(item->data(Qt::UserRole));
            m_menu->removeItem(i);
            m_list->removeRow(row);
        }
    }
    m_deleteButton->setEnabled(false);
    emit contentUpdated("Menuitem deleted");
}

void MenuEditor::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    //emit editContent(item);
}
