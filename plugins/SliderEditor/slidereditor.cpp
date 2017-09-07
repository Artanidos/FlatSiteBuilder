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

#include "slidereditor.h"
#include "slideeditor.h"
#include "tablecheckbox.h"
#include "flatbutton.h"
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QTest>

SliderEditor::SliderEditor()
{
    m_changed = false;
    setAutoFillBackground(true);

    QGridLayout *grid = new QGridLayout();
    grid->setMargin(0);

    m_adminlabel = new QLineEdit;
    m_adminlabel->setMaximumWidth(200);
    QLabel *titleLabel = new QLabel("Slider Module");
    QFont fnt = titleLabel->font();
    fnt.setPointSize(16);
    fnt.setBold(true);
    titleLabel->setFont(fnt);

    FlatButton *close = new FlatButton(":/images/close_normal.png", ":/images/close_hover.png");
    close->setToolTip("Close Editor");

    QPushButton *addSlide = new QPushButton("Add Slide");
    addSlide->setMaximumWidth(120);

    m_list = new QTableWidget(0, 2, this);
    m_list->verticalHeader()->hide();
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_list->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch );
    m_list->setToolTip("Double click to edit item");
    QStringList labels;
    labels << "" << "Name";
    m_list->setHorizontalHeaderLabels(labels);

    m_deleteButton = new QPushButton();
    m_deleteButton->setText("Delete");
    m_deleteButton->setMaximumWidth(120);
    m_deleteButton->setEnabled(false);
    m_deleteButton->setToolTip("Delete all marked items");

    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(close, 0, 2, 1, 1, Qt::AlignRight);
    grid->addWidget(addSlide, 1, 0);
    grid->addWidget(m_list, 2, 0, 1, 3);
    grid->addWidget(m_deleteButton, 3, 0);
    grid->addWidget(new QLabel("Admin Label"), 4, 0);
    grid->addWidget(m_adminlabel, 5, 0);

    setLayout(grid);

    connect(addSlide, SIGNAL(clicked(bool)), this, SLOT(addSlide()));
    connect(m_adminlabel, SIGNAL(textChanged(QString)), this, SLOT(contentChanged()));
    connect(close, SIGNAL(clicked()), this, SLOT(closeEditor()));
    connect(m_list, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(tableDoubleClicked(int, int)));
    connect(m_deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
}

void SliderEditor::setContent(QDomElement element)
{
    m_element = element;
    m_adminlabel->setText(m_element.attribute("adminlabel"));
    m_list->setRowCount(0);

    QDomElement slide = m_element.firstChildElement("Slide");
    while(!slide.isNull())
    {
        Slide *s = new Slide();
        s->setSource(slide.attribute("src"));
        s->setAdminLabel(slide.attribute("adminlabel"));
        addListItem(s);
        slide = slide.nextSiblingElement("Slide");
    }
    m_changed = false;
}

void SliderEditor::closeEditor()
{
    if(m_changed)
    {
        m_element = m_doc.createElement("Slider");
        m_element.setAttribute("adminlabel", m_adminlabel->text());

        for(int i = 0; i < m_list->rowCount(); i++)
        {
            QTableWidgetItem *item = m_list->item(i, 1);
            Slide *slide = qvariant_cast<Slide*>(item->data(Qt::UserRole));
            QDomElement slideelement = m_doc.createElement("Slide");
            slideelement.setAttribute("src", slide->source());
            slideelement.setAttribute("adminlabel", slide->adminLabel());
            m_element.appendChild(slideelement);
        }
    }
    emit close();
}

void SliderEditor::addSlide()
{
    Slide *slide = new Slide();
    addListItem(slide);
    contentChanged();
    tableDoubleClicked(m_list->rowCount() - 1, 0);
}

void SliderEditor::addListItem(Slide *slide)
{
    int rows = m_list->rowCount();
    m_list->setRowCount(rows + 1);
    TableCheckbox *checkBox = new TableCheckbox();
    connect(checkBox, SIGNAL(checkStateChanged(bool)), this, SLOT(checkStateChanged(bool)));
    m_list->setCellWidget(rows, 0, checkBox);
    m_list->setRowHeight(rows, checkBox->sizeHint().height());
    QTableWidgetItem *titleItem = new QTableWidgetItem(slide->title());
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
    titleItem->setData(Qt::UserRole, QVariant::fromValue(slide));
    m_list->setItem(rows, 1, titleItem);
}

void SliderEditor::checkStateChanged(bool)
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

void SliderEditor::tableDoubleClicked(int r, int)
{
    QTableWidgetItem *item = m_list->item(r, 1);
    Slide *slide = qvariant_cast<Slide*>(item->data(Qt::UserRole));

    m_editor = new SlideEditor();
    m_editor->setSite(m_site);
    m_editor->setSlide(slide);
    connect(m_editor, SIGNAL(close()), this, SLOT(editorClosed()));
    animate(item);
}

void SliderEditor::deleteButtonClicked()
{
    for(int row = m_list->rowCount() - 1; row >= 0; row--)
    {
        TableCheckbox *cb = dynamic_cast<TableCheckbox*>(m_list->cellWidget(row, 0));
        if(cb->checked() == Qt::Checked)
        {
            m_list->removeRow(row);
        }
    }
    m_deleteButton->setEnabled(false);
    contentChanged();
}

void SliderEditor::animate(QTableWidgetItem *item)
{
    m_row = item->row();

    // create a cell widget to get the right position in the table
    m_sourcewidget = new QWidget();
    m_list->setCellWidget(m_row, 1, m_sourcewidget);
    QPoint pos = m_sourcewidget->mapTo(this, QPoint(0,0));

    m_editor->setParent(this);
    m_editor->move(pos);
    m_editor->resize(m_sourcewidget->size());
    m_editor->show();

    m_animationgroup = new QParallelAnimationGroup();
    m_animx = new QPropertyAnimation();
    m_animx->setDuration(300);
    m_animx->setStartValue(pos.x());
    m_animx->setEndValue(0);
    m_animx->setTargetObject(m_editor);
    m_animx->setPropertyName("x");
    m_animationgroup->addAnimation(m_animx);
    m_animy = new QPropertyAnimation();
    m_animy->setDuration(300);
    m_animy->setStartValue(pos.y());
    m_animy->setEndValue(0);
    m_animy->setTargetObject(m_editor);
    m_animy->setPropertyName("y");
    m_animationgroup->addAnimation(m_animy);
    m_animw = new QPropertyAnimation();
    m_animw->setDuration(300);
    m_animw->setStartValue(m_sourcewidget->size().width());
    m_animw->setEndValue(this->size().width());
    m_animw->setTargetObject(m_editor);
    m_animw->setPropertyName("width");
    m_animationgroup->addAnimation(m_animw);
    m_animh = new QPropertyAnimation();
    m_animh->setDuration(300);
    m_animh->setStartValue(m_sourcewidget->size().height());
    m_animh->setEndValue(this->size().height());
    m_animh->setTargetObject(m_editor);
    m_animh->setPropertyName("height");
    m_animationgroup->addAnimation(m_animh);
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    m_animationgroup->start();
}

void SliderEditor::animationFineshedZoomIn()
{

}

void SliderEditor::editorClosed()
{
    QPoint pos = m_sourcewidget->mapTo(this, QPoint(0,0));
    // correct end values in case of resizing the window
    m_animx->setStartValue(pos.x());
    m_animy->setStartValue(pos.y());
    m_animw->setStartValue(m_sourcewidget->size().width());
    m_animh->setStartValue(m_sourcewidget->size().height());
    m_animationgroup->setDirection(QAbstractAnimation::Backward);
    disconnect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomIn()));
    connect(m_animationgroup, SIGNAL(finished()), this, SLOT(animationFineshedZoomOut()));
    m_animationgroup->start();

    QTableWidgetItem *item = m_list->item(m_row, 1);
    item->setData(Qt::UserRole, QVariant::fromValue(m_editor->slide()));
    item->setText(m_editor->slide()->title());
    if(m_editor->changed())
        contentChanged();
}

void SliderEditor::animationFineshedZoomOut()
{
    delete m_animationgroup;
    delete m_editor;
    m_editor = NULL;
}

QString SliderEditor::getHtml(QDomElement ele)
{
    QString html = "<div class=\"fullwidthbanner-container roundedcorners\">\n";
    html += "<div class=\"fullwidthbanner\">\n";
    html += "<ul>\n";
    QDomElement slide = ele.firstChildElement("Slide");
    while(!slide.isNull())
    {
        QString source = slide.attribute("src");
        QString url = source.mid(source.indexOf("assets/images/"));
        html += "<li data-transition=\"incube-horizontal\" data-slotamount=\"5\" data-masterspeed=\"700\">\n";
        html += "<img src=\"" + url + "\" alt=\"\" data-bgfit=\"cover\" data-bgposition=\"center top\" data-bgrepeat=\"no-repeat\">\n";
        html += "</li>\n";
        slide = slide.nextSiblingElement("Slide");
    }
    html += "</ul>\n";
    html += "<div class=\"tp-bannertimer\"></div>\n";
    html += "</div>\n";
    html += "</div>\n";
    return html;
}

QString SliderEditor::pluginStyles()
{
    return "<link href=\"assets/plugins/revolution-slider/css/settings.css\" rel=\"stylesheet\" type=\"text/css\"/>\n";
}

QString SliderEditor::pluginScripts()
{
    QString script = "";
    script += "<script type=\"text/javascript\" src=\"assets/plugins/revolution-slider/js/jquery.themepunch.plugins.min.js\"></script>\n";
    script += "<script type=\"text/javascript\" src=\"assets/plugins/revolution-slider/js/jquery.themepunch.revolution.min.js\"></script>\n";
    script += "<script type=\"text/javascript\" src=\"assets/plugins/revolution-slider/js/slider_revolution.js\"></script>\n";
    return script;
}

void SliderEditor::installAssets(QString assetsPath)
{
    QDir assets(assetsPath);
    assets.mkdir("plugins");
    assets.cd("plugins");
    assets.mkdir("revolution-slider");
    assets.cd("revolution-slider");
    assets.mkdir("css");
    assets.mkdir("js");
    assets.mkdir("assets");
    installFiles(":/css", assetsPath + "/plugins/revolution-slider/css");
    installFiles(":/js", assetsPath + "/plugins/revolution-slider/js");
    installFiles(":/assets", assetsPath + "/plugins/revolution-slider/assets");
}

void SliderEditor::installFiles(QString sourcedir, QString targetdir)
{
    QDir source(sourcedir);
    if(!source.exists())
        return;

    foreach (QString filename, source.entryList(QDir::Files))
    {
        QFile::copy(sourcedir + QDir::separator() + filename, targetdir + QDir::separator() + filename);
    }
}
